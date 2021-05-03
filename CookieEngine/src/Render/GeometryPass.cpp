#include "Core/Math/Mat4.hpp"
#include "D3D11Helper.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Mesh.hpp"
#include "Render/GeometryPass.hpp"


using namespace Cookie::Render;

struct VS_CONSTANT_BUFFER
{
    Cookie::Core::Math::Mat4 viewproj = Cookie::Core::Math::Mat4::Identity();
    Cookie::Core::Math::Mat4 model = Cookie::Core::Math::Mat4::Identity();
};

/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

GeometryPass::GeometryPass(int width, int height):
	posFBO		{ width,height,DXGI_FORMAT_R32G32B32A32_FLOAT},
	normalFBO	{ width,height,DXGI_FORMAT_R32G32B32A32_FLOAT},
	albedoFBO	{ width,height }
{
	InitShader();
    InitState();
    CreateDepth(width, height);
}

GeometryPass::~GeometryPass()
{
    if (depthBuffer)
        depthBuffer->Release();
    if (VShader)
        VShader->Release();
    if (PShader)
        PShader->Release();
    if (CBuffer)
        CBuffer->Release();
    if (ILayout)
        ILayout->Release();
    if (PSampler)
        PSampler->Release();
    if (depthStencilState)
        depthStencilState->Release();
    if (rasterizerState)
        rasterizerState->Release();
}

/*=========================== INIT METHODS ===========================*/

void GeometryPass::InitShader()
{
    ID3DBlob* blob = nullptr;

    std::string source = (const char*)R"(#line 27
    struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV; 
        float3 normal : NORMAL;
        float3 fragPos : FRAGPOS;
    };
    
    cbuffer VS_CONSTANT_BUFFER : register(b0)
    {
        float4x4  viewProj;
        float4x4  model;
    };
    
    VOut main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL)
    {
        VOut output;
    
        output.fragPos  = mul(float4(position,1.0),model).xyz;
        output.position = mul(float4(output.fragPos,1.0), viewProj);
        output.uv       = uv;
        output.normal   = normalize(mul(normal,(float3x3)model));
    
        return output;

    }
    )";

    Render::CompileVertex(source, &blob, &VShader);

    source = (const char*)R"(#line 58

    Texture2D	albedoTex           : register(t0);
    Texture2D	normalTex           : register(t1); 
    Texture2D	metallicRoughness   : register(t2); 

    SamplerState WrapSampler : register(s0);

    struct POut
    {
        float4 position : SV_TARGET0;
        float4 normal : SV_TARGET1; 
        float4 albedo : SV_TARGET2;
    };

    POut main(float4 position : SV_POSITION, float2 uv : UV, float3 normal : NORMAL, float3 fragPos : FRAGPOS) 
    {
        POut pOutput;

        float3 texNormal = normalTex.Sample(WrapSampler,uv).xyz;

        pOutput.position    = float4(fragPos,metallicRoughness.Sample(WrapSampler,uv).x);
        pOutput.normal      = lerp(float4(normal,metallicRoughness.Sample(WrapSampler,uv).y),float4(texNormal,metallicRoughness.Sample(WrapSampler,uv).y),step(0.1,dot(texNormal,texNormal)));
        pOutput.albedo      = float4(albedoTex.Sample(WrapSampler,uv).rgb,1.0);


        return pOutput;
    })";

    Render::CompilePixel(source, &PShader);

    struct Vertex
    {
        Core::Math::Vec3 position;
        Core::Math::Vec2 uv;
        Core::Math::Vec3 normal;
    };

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex,position), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex, uv), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    Render::CreateLayout(&blob, ied, 3, &ILayout);

    VS_CONSTANT_BUFFER buffer = {};

    Render::CreateBuffer(&buffer, sizeof(VS_CONSTANT_BUFFER), &CBuffer);

    D3D11_SAMPLER_DESC samDesc = {};
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.MipLODBias = 0.0f;
    samDesc.MaxAnisotropy = 1;
    samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = 0;

    Render::CreateSampler(&samDesc, &PSampler);

    blob->Release();
}

void GeometryPass::InitState()
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    RendererRemote::device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_FRONT;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterizerState);

    D3D11_BLEND_DESC blenDesc = {  };

    blenDesc.AlphaToCoverageEnable = false;
    blenDesc.IndependentBlendEnable         = false;
    blenDesc.RenderTarget[0].BlendEnable    = false;
    blenDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);
}

void GeometryPass::CreateDepth(int width, int height)
{
    ID3D11Texture2D* depthTexture = nullptr;

    D3D11_TEXTURE2D_DESC depthBufferDesc = {};

    // Set up the description of the depth buffer.
    depthBufferDesc.Width = width;
    depthBufferDesc.Height = height;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    HRESULT result = RendererRemote::device->CreateTexture2D(&depthBufferDesc, NULL, &depthTexture);
    if (FAILED(result))
    {
        printf("Failing Creating Texture %p: %s\n", depthTexture, std::system_category().message(result).c_str());
        return;
    }

    // Initialize the depth stencil view.
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};

    // Set up the depth stencil view description.
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    result = RendererRemote::device->CreateDepthStencilView(depthTexture, &depthStencilViewDesc, &depthBuffer);
    if (FAILED(result))
    {
        printf("Failing Creating depth Buffer %p: %s\n", depthTexture, std::system_category().message(result).c_str());
        return;
    }

    depthTexture->Release();
}

/*=========================== REALTIME METHODS ===========================*/

void GeometryPass::Set()
{
    // Now set the rasterizer state.
    Render::RendererRemote::context->RSSetState(rasterizerState);
    // Set the depth stencil state.
    Render::RendererRemote::context->OMSetDepthStencilState(depthStencilState, 1);
    // Set the Blend State.
    const float blendFactor[4] = { 1.0f,1.0f,1.0f,0.0f };
    Render::RendererRemote::context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    Render::RendererRemote::context->IASetInputLayout(ILayout);
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);
    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);

	ID3D11RenderTargetView* fbos[3] = {posFBO.renderTargetView,normalFBO.renderTargetView,albedoFBO.renderTargetView};

	Render::RendererRemote::context->OMSetRenderTargets(3, fbos, depthBuffer);
}

void GeometryPass::Draw(const Core::Math::Mat4& viewProj, const ECS::Coordinator& coordinator)
{
    const ECS::EntityHandler& entityHandler = *coordinator.entityHandler;
    const ECS::ComponentHandler& components = *coordinator.componentHandler;

    VS_CONSTANT_BUFFER buffer = {};
    buffer.viewproj = viewProj;

    size_t bufferSize = sizeof(buffer);

    ID3D11ShaderResourceView* fbos[3] = { nullptr, nullptr, nullptr};

    

    for (int i = 0; i < entityHandler.livingEntities; ++i)
    {
        if (entityHandler.entities[i].signature & (SIGNATURE_TRANSFORM + SIGNATURE_MODEL))
        {
            buffer.model = components.componentTransforms[entityHandler.entities[i].id].TRS;
            Render::WriteCBuffer(&buffer, bufferSize, 0, &CBuffer);

            const ECS::ComponentModel& model = components.componentModels[entityHandler.entities[i].id];

            if (model.albedo)
                model.albedo->Set(0);
            if (model.normal)
                model.normal->Set(1);
            if (model.metallicRoughness)
                model.metallicRoughness->Set(2);
            if (model.mesh)
            {
                model.mesh->Set();
                model.mesh->Draw();
            }

            Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);
        }
    }
}

void GeometryPass::Clear()
{
	float black[4] = { 0.0f,0.0f,0.0f,0.0f };

	Render::RendererRemote::context->ClearRenderTargetView(posFBO.renderTargetView,		black);
	Render::RendererRemote::context->ClearRenderTargetView(normalFBO.renderTargetView,  black);
	Render::RendererRemote::context->ClearRenderTargetView(albedoFBO.renderTargetView,	black);
}