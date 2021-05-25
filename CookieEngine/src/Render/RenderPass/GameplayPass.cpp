#include "D3D11Helper.hpp"
#include "DrawDataHandler.hpp"
#include "RenderPass/GameplayPass.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;


/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

GameplayPass::GameplayPass()
{
    InitState();
}

GameplayPass::~GameplayPass()
{
    if (rasterizerState)
    {
        rasterizerState->Release();
    }
    if (depthStencilState)
    {
        depthStencilState->Release();
    }
    if (blendState)
    {
        blendState->Release();
    }
    if (PSampler)
    {
        PSampler->Release();
    }
}

/*=========================== INIT METHODS ===========================*/

void GameplayPass::InitState()
{

    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = false;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    RendererRemote::device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = true;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterizerState);

    D3D11_BLEND_DESC blenDesc = {  };

    blenDesc.AlphaToCoverageEnable                  = false;
    blenDesc.IndependentBlendEnable                 = false;
    blenDesc.RenderTarget[0].BlendEnable            = true;
    blenDesc.RenderTarget[0].SrcBlend               = D3D11_BLEND_SRC_ALPHA;
    blenDesc.RenderTarget[0].DestBlend              = D3D11_BLEND_INV_SRC_ALPHA;
    blenDesc.RenderTarget[0].BlendOp                = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha          = D3D11_BLEND_SRC_ALPHA;
    blenDesc.RenderTarget[0].DestBlendAlpha         = D3D11_BLEND_INV_SRC_ALPHA;
    blenDesc.RenderTarget[0].BlendOpAlpha           = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);

    D3D11_SAMPLER_DESC samDesc = {};
    samDesc.Filter      = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samDesc.AddressU    = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.AddressV    = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.AddressW    = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.MipLODBias = 0.0f;
    samDesc.MaxAnisotropy = 1;
    samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = 0;

    Render::CreateSampler(&samDesc, &PSampler);
}

/*=========================== REALTIME METHODS ===========================*/

void GameplayPass::Set()
{
    // Now set the rasterizer state.
    Render::RendererRemote::context->RSSetState(rasterizerState);
    // Set the depth stencil state.
    Render::RendererRemote::context->OMSetDepthStencilState(depthStencilState, 1);
    // Set the Blend State.
    const float blendFactor[4] = { 1.0f,1.0f,1.0f,0.0f };
    Render::RendererRemote::context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);
}

void GameplayPass::Draw(const DrawDataHandler& drawData)
{
    if (drawData.player && drawData.currentCam)
    {
        playerDrawer.Set(drawData);
        playerDrawer.Draw();
    }
}