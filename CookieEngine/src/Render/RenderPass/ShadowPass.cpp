#include "Render/D3D11Helper.hpp"
#include "Light.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Resources/Mesh.hpp"
#include "Render/ShadowBuffer.hpp"
#include "RenderPass/ShadowPass.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

struct VS_CONSTANT_BUFFER
{
    Cookie::Core::Math::Mat4 lightViewProj  = Cookie::Core::Math::Mat4::Identity();
    Cookie::Core::Math::Mat4 model          = Cookie::Core::Math::Mat4::Identity();
};

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

ShadowPass::ShadowPass()
{
    InitShader();
    InitState();
}

ShadowPass::~ShadowPass()
{
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
    cbuffer VS_CONSTANT_BUFFER : register(b0)
    {
        float4x4  lightViewProj;
        float4x4  model;
    };
    
    float4 main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL) : SV_POSITION
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
    rasterDesc.CullMode = D3D11_CULL_BACK;
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

    ID3D11ShaderResourceView* fbos[4] = { nullptr,nullptr,nullptr};

    Render::RendererRemote::context->PSSetShaderResources(0, 4, fbos);

    RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    RendererRemote::context->PSSetShader(nullptr, nullptr, 0);

    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);
}

void ShadowPass::Draw(const ECS::Coordinator& coordinator, Resources::Map& map, LightsArray& lights)
{
    const ECS::EntityHandler& entityHandler = *coordinator.entityHandler;
    const ECS::ComponentHandler& components = *coordinator.componentHandler;

    VS_CONSTANT_BUFFER buffer = {};

    size_t bufferSize = sizeof(buffer);

    Core::Math::Mat4 proj = Core::Math::Mat4::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 1000.0f);

    for (int j = 0; j < lights.usedDir; j++)
    {
        DirLight& jLight = lights.dirLights.at(j);
        if (jLight.castShadow)
        {
            Render::RendererRemote::context->OMSetRenderTargets(0, nullptr, jLight.shadowMap->depthStencilView);
            jLight.lightViewProj = proj * Core::Math::Mat4::LookAt(jLight.dir.Normalize() * 10.0f, {0.0f,0.0f,0.0f}, { 0.0f,1.0f,0.0f });
            buffer.lightViewProj = jLight.lightViewProj;

            for (int i = 0; i < entityHandler.livingEntities; ++i)
            {
                if (entityHandler.entities[i].signature & (SIGNATURE_TRANSFORM + SIGNATURE_MODEL))
                {
                    buffer.model = components.componentTransforms[entityHandler.entities[i].id].TRS;
                    Render::WriteCBuffer(&buffer, bufferSize, 0, &CBuffer);

                    const Resources::Mesh* mesh = components.componentModels[entityHandler.entities[i].id].mesh;

                    if (mesh)
                    {
                        mesh->Set();
                        mesh->Draw();
                    }
                }
            }
        }
    }
}