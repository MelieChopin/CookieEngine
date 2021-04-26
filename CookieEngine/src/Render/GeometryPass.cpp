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
	posFBO		{ width,height},
	normalFBO	{ width,height },
	albedoFBO	{ width,height }
{
	InitShader();
}

GeometryPass::~GeometryPass()
{
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

    Texture2D	diffuseTex2D : register(t0);    

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

        pOutput.position    = float4(fragPos,1.0);
        pOutput.normal      = float4(normal,1.0);
        pOutput.albedo      = float4(diffuseTex2D.Sample(WrapSampler,uv).rgb,1.0);


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
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, normal), D3D11_INPUT_PER_VERTEX_DATA, 0}
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
}

/*=========================== REALTIME METHODS ===========================*/

void GeometryPass::Set(ID3D11DepthStencilView* depthStencilView)
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    Render::RendererRemote::context->IASetInputLayout(ILayout);
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);
    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);

	ID3D11RenderTargetView* fbos[3] = {posFBO.renderTargetView,normalFBO.renderTargetView,albedoFBO.renderTargetView};

	Render::RendererRemote::context->OMSetRenderTargets(3, fbos, depthStencilView);
}

void GeometryPass::Draw(const Core::Math::Mat4& viewProj, const ECS::Coordinator& coordinator)
{
    const ECS::EntityHandler& entityHandler = *coordinator.entityHandler;
    const ECS::ComponentHandler& components = *coordinator.componentHandler;

    VS_CONSTANT_BUFFER buffer = {};
    buffer.viewproj = viewProj;

    size_t bufferSize = sizeof(buffer);

    for (int i = 0; i < entityHandler.livingEntities; ++i)
    {
        if (entityHandler.entities[i].signature & (SIGNATURE_TRANSFORM + SIGNATURE_MODEL))
        {
            buffer.model = components.componentTransforms[entityHandler.entities[i].id].TRS;
            Render::WriteCBuffer(&buffer, bufferSize, 0, &CBuffer);

            const ECS::ComponentModel& model = components.componentModels[entityHandler.entities[i].id];

            model.texture->Set();
            model.mesh->Set();
            model.mesh->Draw();
        }
    }
}

void GeometryPass::Clear(const Core::Math::Vec4& clearColor)
{
	float black[4] = { 0.0f,0.0f,0.0f,0.0f };

	Render::RendererRemote::context->ClearRenderTargetView(posFBO.renderTargetView,		black);
	Render::RendererRemote::context->ClearRenderTargetView(normalFBO.renderTargetView,  black);
	Render::RendererRemote::context->ClearRenderTargetView(albedoFBO.renderTargetView,	clearColor.e);
}