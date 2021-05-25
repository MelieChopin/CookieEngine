#include "Render/D3D11Helper.hpp"
#include "Core/Math/Vec4.hpp"
#include "Light.hpp"
#include "RenderPass/LightPass.hpp"
#include "Render/DrawDataHandler.hpp"
#include "Camera.hpp"
#include "ShadowBuffer.hpp"

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
    if (volumeRasterState)
        volumeRasterState->Release();
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

    Render::CreateBuffer(&cam, sizeof(Vec4) * 2, &lightCBuffer);
}

void LightPass::InitState()
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable    = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc      = D3D11_COMPARISON_GREATER_EQUAL;

    depthStencilDesc.StencilEnable      = false;
    depthStencilDesc.StencilReadMask    = 0xFF;
    depthStencilDesc.StencilWriteMask   = 0xFF;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp        = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp   = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilPassOp        = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc          = D3D11_COMPARISON_EQUAL;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp         = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp    = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilPassOp         = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc           = D3D11_COMPARISON_EQUAL;

    RendererRemote::device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = true;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterizerState);


    rasterDesc.CullMode = D3D11_CULL_FRONT;
    RendererRemote::device->CreateRasterizerState(&rasterDesc, &volumeRasterState);

    D3D11_BLEND_DESC blenDesc = {  };

    blenDesc.AlphaToCoverageEnable          = false;
    blenDesc.IndependentBlendEnable         = false;
    blenDesc.RenderTarget[0].BlendEnable    = true;
    blenDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_MAX;
    blenDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);
}

/*======================= REALTIME METHODS =======================*/

void LightPass::Set(FrameBuffer& posFBO, FrameBuffer& normalFBO, FrameBuffer& albedoFBO)
{
    // Now set the rasterizer state.
    Render::RendererRemote::context->RSSetState(rasterizerState);
    // Set the depth stencil state.
    Render::RendererRemote::context->OMSetDepthStencilState(depthStencilState, 0);
    // Set the Blend State.
    const float blendFactor[4] = { 1.0f,1.0f,1.0f,0.0f };
    Render::RendererRemote::context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    ID3D11ShaderResourceView* fbos[3] = { posFBO.shaderResource,normalFBO.shaderResource, albedoFBO.shaderResource};

    Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);

    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);
}

void LightPass::Draw(const LightsArray& lights, const ShadowBuffer& shadowMap, const DrawDataHandler& drawData)
{
    const Camera& cam = *drawData.currentCam;
    ID3D11RenderTargetView* rtvs[2] = { diffuseFBO.renderTargetView,specularFBO.renderTargetView };


    Vec4 camBuffer[2] = { { cam.pos.x,cam.pos.y ,cam.pos.z ,0.0f },{static_cast<float>(diffuseFBO.width),static_cast<float>(diffuseFBO.height),cam.camFar,0.0f} };

    Render::WriteCBuffer(&camBuffer, sizeof(Vec4) * 2, 0, &lightCBuffer);

    if (lights.useDir)
    {
        Render::RendererRemote::context->OMSetRenderTargets(2, rtvs, nullptr);
        dirDrawer.Set(lights.dirLight, shadowMap, &lightCBuffer);
        Render::RendererRemote::context->Draw(3, 0);
    }
    if (lights.usedPoints > 0)
    {

        pointDrawer.Set(&lightCBuffer, lights, drawData);

        Render::RendererRemote::context->RSSetState(volumeRasterState);
        Render::RendererRemote::context->OMSetRenderTargets(2, rtvs, drawData.depthStencilView);

        pointDrawer.Draw(lights.usedPoints);
    }
    
}

void LightPass::Clear()
{
    Core::Math::Vec4 clearColor = { 0.0f,0.0f,0.0f,1.0f };

     Render::RendererRemote::context->ClearRenderTargetView(diffuseFBO.renderTargetView, clearColor.e);
     Render::RendererRemote::context->ClearRenderTargetView(specularFBO.renderTargetView, clearColor.e);
}