#include "Render/D3D11Helper.hpp"
#include "Light.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Resources/Mesh.hpp"
#include "Render/ShadowBuffer.hpp"
#include "Render/DrawDataHandler.hpp"
#include "Camera.hpp"
#include "RenderPass/ShadowPass.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

struct VS_CONSTANT_BUFFER
{
    Mat4 lightViewProj  = Mat4::Identity();
};

constexpr float shadowProjEpsilon = 100.0f;

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

ShadowPass::ShadowPass():
    shadowViewport{0.0f,0.0f,SHADOW_MAP_RESOLUTION,SHADOW_MAP_RESOLUTION,0.0f,1.0f}
{
    InitShader();
    InitState();
    proj = Mat4::Ortho(-shadowProjEpsilon, shadowProjEpsilon, -shadowProjEpsilon, shadowProjEpsilon, -shadowProjEpsilon, shadowProjEpsilon);
}

ShadowPass::~ShadowPass()
{
    if (VShader)
        VShader->Release();
    if (CBuffer)
        CBuffer->Release();
    if (blendState)
        blendState->Release();
    if (depthStencilState)
        depthStencilState->Release();
    if (rasterizerState)
        rasterizerState->Release();
}

/*======================= INIT METHODS =======================*/

void ShadowPass::InitShader()
{

    ID3DBlob* blob = nullptr;

    std::string source = (const char*)R"(#line 35

    cbuffer CAM_CONSTANT : register(b0)
    {
        float4x4  lightViewProj;
    };
    
    float4 main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL, float4x4 model : WORLD) : SV_POSITION
    {
        float4 pos;
    
        pos = mul(float4(position,1.0),model);
        pos = mul(pos, lightViewProj);

        return pos;

    }
    )";

    CompileVertex(source, &blob, &VShader);

    VS_CONSTANT_BUFFER buffer = {};

    Render::CreateBuffer(&buffer, sizeof(VS_CONSTANT_BUFFER), &CBuffer);

    blob->Release();
}

void ShadowPass::InitState()
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable    = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable      = false;
    depthStencilDesc.StencilReadMask    = 0xFF;
    depthStencilDesc.StencilWriteMask   = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc          = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc           = D3D11_COMPARISON_ALWAYS;

    RendererRemote::device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_NONE;
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
    blenDesc.IndependentBlendEnable = false;
    blenDesc.RenderTarget[0].BlendEnable = false;
    blenDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);
}

/*======================= REALTIME METHODS =======================*/

void ShadowPass::Set()
{
    // Now set the rasterizer state.
    Render::RendererRemote::context->RSSetState(rasterizerState);
    // Set the depth stencil state.
    Render::RendererRemote::context->OMSetDepthStencilState(depthStencilState, 0);
    // Set the Blend State.
    const float blendFactor[4] = { 1.0f,1.0f,1.0f,0.0f };
    Render::RendererRemote::context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    ID3D11RenderTargetView* rtvs[4] = { nullptr,nullptr ,nullptr ,nullptr };

    Render::RendererRemote::context->OMSetRenderTargets(4, rtvs, nullptr);

    ID3D11ShaderResourceView* fbos[4] = { nullptr,nullptr,nullptr, nullptr};

    Render::RendererRemote::context->PSSetShaderResources(0, 4, fbos);

    RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    RendererRemote::context->PSSetShader(nullptr, nullptr, 0);

    
}

void ShadowPass::Draw(DrawDataHandler& drawData)
{
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);

    VS_CONSTANT_BUFFER buffer = {};
    

    size_t bufferSize = sizeof(buffer);

    Render::RendererRemote::context->RSSetViewports(1, &shadowViewport);

    if (drawData.lights->useDir && drawData.lights->dirLight.castShadow)
    {
        DirLight& dirLight = drawData.lights->dirLight;

        Render::RendererRemote::context->OMSetRenderTargets(0, nullptr, shadowMap.depthStencilView);

        Vec3 jDir   = dirLight.dir.Normalize();
        Vec3 pos    = (drawData.AABB[0] + drawData.AABB[1]) * 0.5f;
        Mat4 view   = Mat4::LookAt({ 0.0f,0.0f,0.0f }, jDir, { 0.0f,1.0f,0.0f });

        view = Mat4::Translate(-(pos - jDir * shadowProjEpsilon * 0.5f)) * view;

        dirLight.lightViewProj  = view * proj;
        buffer.lightViewProj    = dirLight.lightViewProj;
        Render::WriteBuffer(&buffer, bufferSize, 0, &CBuffer);

        drawData.Draw(true);
        
    }
}

void ShadowPass::Clear()
{
    shadowMap.Clear();
}