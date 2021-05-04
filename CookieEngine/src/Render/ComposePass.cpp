#include "D3D11Helper.hpp"
#include "FrameBuffer.hpp"
#include "ComposePass.hpp"

using namespace Cookie::Render;

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

ComposePass::ComposePass()
{
    InitShader();
    InitState();
}

ComposePass::~ComposePass()
{

}

/*======================= INIT METHODS =======================*/

void ComposePass::InitShader()
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
    
        float2 uv       = float2((vI << 1) & 2, vI & 2);
        output.uv       = uv;
        output.position = float4(uv.x * 2 - 1, -uv.y * 2 + 1, 0, 1);
    
        return output;
    }
    )";

    CompileVertex(source, &blob, &VShader);

    source = (const char*)R"(#line 58

    Texture2D	diffuseTex  : register(t0);    
    Texture2D	specularTex : register(t1);  

    SamplerState ClampSampler : register(s0);

    float4 main(float4 position : SV_POSITION, float2 uv : UV) : SV_TARGET0
    {
        float3  finalColor    = pow(diffuseTex.Sample(ClampSampler,uv).xyz + specularTex.Sample(ClampSampler,uv).xyz,2.2);

        float3 denominator = (finalColor + 1.0);

        float3 mapped = finalColor/denominator;

        return float4(pow(mapped,0.454545),1.0);
    })";

    CompilePixel(source, &PShader);

    D3D11_SAMPLER_DESC samDesc = {};
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    samDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.MipLODBias = 0.0f;
    samDesc.MaxAnisotropy = 1;
    samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samDesc.BorderColor[0] = samDesc.BorderColor[1] = samDesc.BorderColor[2] = samDesc.BorderColor[3] = 0;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = 0;

    Render::CreateSampler(&samDesc, &PSampler);

    blob->Release();
}

void ComposePass::InitState()
{
    // Initialize the description of the stencil state.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};

    // Set up the description of the stencil state.
    depthStencilDesc.DepthEnable    = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable      = true;
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
    rasterDesc.AntialiasedLineEnable    = false;
    rasterDesc.CullMode                 = D3D11_CULL_BACK;
    rasterDesc.DepthBias                = 0;
    rasterDesc.DepthBiasClamp           = 0.0f;
    rasterDesc.DepthClipEnable          = false;
    rasterDesc.FillMode                 = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise    = false;
    rasterDesc.MultisampleEnable        = false;
    rasterDesc.ScissorEnable            = false;
    rasterDesc.SlopeScaledDepthBias     = 0.0f;

    RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterizerState);

    D3D11_BLEND_DESC blenDesc = {  };

    blenDesc.AlphaToCoverageEnable                  = false;
    blenDesc.IndependentBlendEnable                 = false;
    blenDesc.RenderTarget[0].BlendEnable            = false;
    blenDesc.RenderTarget[0].SrcBlend               = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlend              = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOp                = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha          = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlendAlpha         = D3D11_BLEND_ZERO;
    blenDesc.RenderTarget[0].BlendOpAlpha           = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);
}

/*======================= REALTIME METHODS =======================*/


void ComposePass::Set(FrameBuffer& diffuse, FrameBuffer& specular)
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

    Render::RendererRemote::context->IASetInputLayout(nullptr);
    Render::RendererRemote::context->PSSetSamplers(0, 1, &PSampler);

    ID3D11ShaderResourceView* fbos[2] = {diffuse.shaderResource, specular.shaderResource };

    Render::RendererRemote::context->PSSetShaderResources(0, 2, fbos);
}

void ComposePass::Draw()
{
    Render::RendererRemote::context->Draw(3, 0);
}