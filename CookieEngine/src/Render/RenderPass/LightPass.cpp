#include "Render/D3D11Helper.hpp"
#include "Core/Math/Vec4.hpp"
#include "Light.hpp"
#include "RenderPass/LightPass.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

LightPass::LightPass(int width, int height) :
    diffuseFBO{ width, height, DXGI_FORMAT_R32G32B32A32_FLOAT },
    specularFBO {width, height, DXGI_FORMAT_R32G32B32A32_FLOAT}
{
    InitShader();
    InitState();
}

LightPass::~LightPass()
{
    if (lightCBuffer)
        lightCBuffer->Release();
    if (PSampler)
        PSampler->Release();
    if (depthStencilState)
        depthStencilState->Release();
    if (rasterizerState)
        rasterizerState->Release();
    if (blendState)
        blendState->Release();
}

/*======================= INIT METHODS =======================*/

void LightPass::InitShader()
{
    Vec4 cam = {};

    Render::CreateBuffer(&cam, sizeof(Vec4), &lightCBuffer);

    D3D11_SAMPLER_DESC samDesc = {};
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.MipLODBias = 0.0f;
    samDesc.MaxAnisotropy = 0.0f;
    samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0.0f;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = 0;

    Render::CreateSampler(&samDesc, &PSampler);

    Render::CreateBuffer(&cam, sizeof(Vec4), &lightCBuffer);
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

    Vec4 cam = {camPos.x,camPos.y ,camPos.z ,0.0f };
    Render::WriteCBuffer(&cam, sizeof(Vec4), 0, &lightCBuffer);

    ID3D11RenderTargetView* rtvs[2] = { diffuseFBO.renderTargetView,specularFBO.renderTargetView};

    Render::RendererRemote::context->OMSetRenderTargets(2, rtvs, nullptr);

    ID3D11ShaderResourceView* fbos[3] = { posFBO.shaderResource,normalFBO.shaderResource, albedoFBO.shaderResource};

    Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);

    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);
}

void LightPass::Draw(const LightsArray& lights)
{
    dirLight.Set(&lightCBuffer);
    for (int i = 0; i < lights.usedDir; i++)
    {
        dirLight.Write(lights.dirLights[i]);
        Render::RendererRemote::context->Draw(3, 0);
    }
}

void LightPass::Clear()
{
    Core::Math::Vec4 clearColor = { 0.0f,0.0f,0.0f,1.0f };

     Render::RendererRemote::context->ClearRenderTargetView(diffuseFBO.renderTargetView, clearColor.e);
     Render::RendererRemote::context->ClearRenderTargetView(specularFBO.renderTargetView, clearColor.e);
}