#include <d3d11.h>
#include <d3dcompiler.h>
#include <system_error>

#include "Vec4.hpp"
#include "Mat4.hpp"
#include "Render/Renderer.hpp"
#include "Render/RendererRemote.hpp"
#include "Resources/Shader/FBODrawShader.hpp"

using namespace Cookie::Resources;

FBODrawShader::FBODrawShader(std::string _name) :
    Shader(_name)
{
    ID3DBlob* VS;

    if (CompileVertex(&VS) && CompilePixel())
        CreateSampler();

}

FBODrawShader::~FBODrawShader()
{
    if (PShader)
        PShader->Release();
    if (VShader)
        VShader->Release();
    if (layout)
        layout->Release();
    if (sampler)
        sampler->Release();
}

std::string FBODrawShader::GetVertexSource()
{
    return (const char*)R"(struct VOut
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
}

std::string FBODrawShader::GetPixelSource()
{
    return (const char*)R"(#line 75

    Texture2D	diffuseTex2D : register(t0);    

    SamplerState WrapSampler : register(s0);

    float4 main(float4 position : SV_POSITION, float2 uv : UV) : SV_TARGET
    {
        return diffuseTex2D.Sample(WrapSampler,uv);        
    })";
}


bool FBODrawShader::CompileVertex(ID3DBlob** VS)
{
    ID3DBlob* VSErr;
    std::string source = GetVertexSource();

    if (FAILED(D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, VS, &VSErr)))
    {
        printf("%s\n", (const char*)(VSErr->GetBufferPointer()));
        return false;
    }

    if (FAILED(Render::RendererRemote::device->CreateVertexShader((*VS)->GetBufferPointer(), (*VS)->GetBufferSize(), NULL, &VShader)))
    {
        printf("Failed Creating Vertex Shader \n");
        return false;
    }

    return true;
}

bool FBODrawShader::CompilePixel()
{
    ID3DBlob* PS;
    ID3DBlob* PSErr;

    std::string source = GetPixelSource();

    if (FAILED(D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &PS, &PSErr)))
    {
        printf("%s\n", (const char*)(PSErr->GetBufferPointer()));
        return false;
    }

    if (FAILED(Render::RendererRemote::device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &PShader)))
    {
        printf("Failed Creating Pixel Shader \n");
        return false;
    }

    return true;
}

bool FBODrawShader::CreateLayout(ID3DBlob** VS)
{
    return true;
}

bool FBODrawShader::CreateBuffer()
{
    return true;
}


bool FBODrawShader::CreateSampler()
{
    // Create a sampler state
    D3D11_SAMPLER_DESC SamDesc = {};
    SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamDesc.MipLODBias = 0.0f;
    SamDesc.MaxAnisotropy = 1;
    SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
    SamDesc.MinLOD = 0;
    SamDesc.MaxLOD = 0;

    if (FAILED(Render::RendererRemote::device->CreateSamplerState(&SamDesc, &sampler)))
    {
        printf("Failed Creating sampler state");
        return false;
    }

    return true;
}

void FBODrawShader::Set()
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    Render::RendererRemote::context->IASetInputLayout(nullptr);

    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);
    Render::RendererRemote::context->PSSetSamplers(0, 1, &sampler);

    Render::RendererRemote::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    Render::RendererRemote::currentShader = (Shader*)this;
}

void FBODrawShader::WriteCBuffer(const Core::Math::Mat4& proj, const Core::Math::Mat4& view)
{
}