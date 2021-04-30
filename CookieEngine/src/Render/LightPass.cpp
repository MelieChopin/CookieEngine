#include "Render/D3D11Helper.hpp"
#include "Core/Math/Vec4.hpp"
#include "Light.hpp"
#include "LightPass.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

struct PS_DIRLIGHT_BUFFER
{
    Vec3 dir;
    float padding = 0.0f;
    Vec3 color = { 1.0f,1.0f,1.0f };
    float padding2 = 0.0f;
};

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

LightPass::LightPass()
{
    lights.dirLights[0] = { {0.0f,-1.0f,0.0f},{0.25f,0.25f,0.25f} };
    lights.usedDir++;
    lights.dirLights[1] = { {-1.0f,1.0f,0.0f},{0.25f,0.25f,0.25f} };
    lights.usedDir++;
    lights.dirLights[2] = { {0.0f,-1.0f,1.0f},{0.1f,0.1f,0.0f} };
    lights.usedDir++;
    //lights.dirLights[3] = { {0.0f,-1.0f,-1.0f},{0.25f,0.25f,0.25f} };
    //lights.usedDir++;
    InitShader();
    InitState();
}

LightPass::~LightPass()
{
    if (dirVShader)
        dirVShader->Release();
    if (dirPShader)
        dirPShader->Release();
    if (dirCBuffer)
        dirCBuffer->Release();
    if (PSampler)
        PSampler->Release();
    if (depthStencilState)
        depthStencilState->Release();
    if (rasterizerState)
        rasterizerState->Release();
}

/*======================= INIT METHODS =======================*/

void LightPass::InitShader()
{
    ID3DBlob* blob = nullptr;

    std::string source = (const char*)R"(#line 30
    struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV;
    };
    
    VOut main(uint vI :SV_VertexId )
    {
        VOut output;
    
        float2 uv = float2((vI << 1) & 2, vI & 2);
        output.uv = float2(1-uv.x,uv.y);
        output.position = float4(-uv.x * 2 + 1, -uv.y * 2 + 1, 0, 1);
    
        return output;

    }
    )";

    Render::CompileVertex(source, &blob, &dirVShader);

    source = std::string(blinnPhong) + std::string((const char*)R"(#line 58

    Texture2D	positionTex : register(t0);
    Texture2D	normalTex   : register(t1); 
    Texture2D	albedoTex   : register(t2);     

    cbuffer DirLight : register(b0)
    {
        float3 lightDir;
        float3 lightColor;
    };

    SamplerState ClampSampler : register(s0);

    float4 main(float4 position : SV_POSITION, float2 uv : UV) : SV_TARGET0
    {
        float3  fragPos     = positionTex.Sample(ClampSampler,uv).xyz;
        float3  normal      = normalize(normalTex.Sample(ClampSampler,uv).xyz);
        float4  color       = albedoTex.Sample(ClampSampler,uv);
        float   metalic     = positionTex.Sample(ClampSampler,uv).w;
        float   roughness   = 0.9;//normalTex.Sample(ClampSampler,uv).w;

        float3 luminance = compute_lighting(normal,fragPos,normalize(-lightDir),metalic,roughness * roughness) * lightColor;

        return color * float4(luminance,1.0);
    })");

    Render::CompilePixel(source, &dirPShader);

    PS_DIRLIGHT_BUFFER buffer = {};

    Render::CreateBuffer(&buffer, sizeof(PS_DIRLIGHT_BUFFER), &dirCBuffer);

    Vec4 cam = {};

    Render::CreateBuffer(&cam, sizeof(Vec4), &lightCBuffer);

    D3D11_SAMPLER_DESC samDesc = {};
    samDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samDesc.AddressU    = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.AddressV    = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.AddressW    = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.MipLODBias = 0.0f;
    samDesc.MaxAnisotropy = 1;
    samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = 0;

    Render::CreateSampler(&samDesc, &PSampler);

    blob->Release();
}

void LightPass::InitState()
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

    blenDesc.AlphaToCoverageEnable          = false;
    blenDesc.IndependentBlendEnable         = false;
    blenDesc.RenderTarget[0].BlendEnable    = true;
    blenDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);
}

/*======================= REALTIME METHODS =======================*/

void LightPass::Set(FrameBuffer& posFBO, FrameBuffer& normalFBO, FrameBuffer& albedoFBO, const Core::Math::Vec3& camPos)
{
    // Now set the rasterizer state.
    Render::RendererRemote::context->RSSetState(rasterizerState);
    // Set the depth stencil state.
    Render::RendererRemote::context->OMSetDepthStencilState(depthStencilState, 1);
    // Set the Blend State.
    const float blendFactor[4] = { 1.0f,1.0f,1.0f,0.0f };
    Render::RendererRemote::context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    Render::RendererRemote::context->VSSetShader(dirVShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(dirPShader, nullptr, 0);

    Render::RendererRemote::context->IASetInputLayout(nullptr);
    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);

    ID3D11Buffer* buffer[] = { dirCBuffer, lightCBuffer };

    Render::RendererRemote::context->PSSetConstantBuffers(0, 2, buffer);

    Vec4 cam = {camPos.x,camPos.y ,camPos.z ,0.0f };
    Render::WriteCBuffer(&cam, sizeof(Vec4), 0, &lightCBuffer);

    ID3D11ShaderResourceView* fbos[3] = { posFBO.shaderResource,normalFBO.shaderResource,albedoFBO.shaderResource};

    Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);
}

void LightPass::Draw(FrameBuffer& fbo)
{
    Render::RendererRemote::context->OMSetRenderTargets(1, &fbo.renderTargetView, nullptr);
    for (int i = 0; i < lights.usedDir; i++)
    {
        PS_DIRLIGHT_BUFFER buffer = { lights.dirLights[i].dir,0.0f, lights.dirLights[i].color,0.0f};
        Render::WriteCBuffer(&buffer, sizeof(PS_DIRLIGHT_BUFFER), 0, &dirCBuffer);
        Render::RendererRemote::context->Draw(3, 0);
    }
}