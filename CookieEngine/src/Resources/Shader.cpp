#include <d3d11.h>
#include <d3dcompiler.h>

#include "Vec4.hpp"
#include "Render/Renderer.hpp"
#include "Resources/Shader.hpp"

using namespace Cookie::Resources;

Shader::Shader(Render::Renderer& _renderer)
{
    ID3DBlob* VS;

    if (CompileDefaultVertex(_renderer,&VS) && CompileDefaultPixel(_renderer))
        CreateDefaultLayout(_renderer, &VS);
}

Shader::Shader(Render::Renderer& _renderer, std::string VShaderPath, std::string PShaderPath)
{
    ID3DBlob* VS;

    if (CompileVertex(_renderer,&VS,VShaderPath) && CompilePixel(_renderer,PShaderPath))
        CreateDefaultLayout(_renderer, &VS);
}

Shader::~Shader()
{
    if (PShader)
        PShader->Release();
    if (VShader)
        VShader->Release();
    if (layout)
        layout->Release();
}

std::string Shader::GetDefaultVertexSource()
{
	return (const char*)R"(struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV; 
        float3 normal : NORMAL;
    };
    
    cbuffer VS_CONSTANT_BUFFER : register(b0)
    {
        float4x4  projection;
        float4x4  view;
        float4x4  model;
    };
    
    VOut main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL)
    {
        VOut output;
    
        output.position = mul(mul(mul(float4(position,1.0), model),view),projection);
        output.uv       = uv;
        output.normal   = mul(float4(normal,0.0), model).xyz;
    
        return output;

    })";
}

std::string Shader::GetDefaultPixelSource()
{
	return (const char*)R"(
    Texture2D	diffuseTex2D : register( t0 );
    Texture2D	emmisiveTex2D : register( t1 );

    SamplerState linearSampler : register( s0 );

    float4 main(float4 position : SV_POSITION, float2 uv : UV, float3 normal : NORMAL) : SV_TARGET
    {
        return diffuseTex2D.Sample(linearSampler,uv) + emmisiveTex2D.Sample(linearSampler,uv);
    })";
}



bool Shader::CompileDefaultVertex(Render::Renderer& _renderer, ID3DBlob** VS)
{
    ID3DBlob* VSErr;
    std::string source = GetDefaultVertexSource();

    if (FAILED(D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, VS, &VSErr)))
    {
        printf("%s\n", (const char*)(VSErr->GetBufferPointer()));
        return false;
    }

    return _renderer.CreateVertexBuffer(&VShader,VS);
}

bool Shader::CompileDefaultPixel(Render::Renderer& _renderer)
{
    ID3DBlob* PS;
    ID3DBlob* PSErr;

    std::string source = GetDefaultPixelSource();

    if (FAILED(D3DCompile(source.c_str(), source.length(), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &PS, &PSErr)))
    {
        printf("%s\n", (const char*)(PSErr->GetBufferPointer()));
        return false;
    }

    return _renderer.CreatePixelBuffer(&PShader, &PS);
}

bool Shader::CreateDefaultLayout(Render::Renderer& _renderer, ID3DBlob** VS)
{
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

    if (FAILED(_renderer.GetDevice()->CreateInputLayout(ied, 3, (*VS)->GetBufferPointer(), (*VS)->GetBufferSize(), &layout)))
    {
        printf("FAILED to create input layout \n");
        return false;
    }

    return true;
}

bool Shader::CompileVertex(Render::Renderer& _renderer, ID3DBlob** VS, std::string VShaderPath)
{
    ID3DBlob* VSErr;

    if (FAILED(D3DCompileFromFile((LPCWSTR)VShaderPath.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, VS, &VSErr)))
    {
        printf("%s\n", (const char*)(VSErr->GetBufferPointer()));
        return false;
    }

    return _renderer.CreateVertexBuffer(&VShader, VS);
}

bool Shader::CompilePixel(Render::Renderer& _renderer, std::string PShaderPath)
{

    ID3DBlob* PS;
    ID3DBlob* PSErr;

    if (FAILED(D3DCompileFromFile((LPCWSTR)PShaderPath.c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &PS, &PSErr)))
    {
        printf("%s\n", (const char*)(PSErr->GetBufferPointer()));
        return false;
    }

    return _renderer.CreatePixelBuffer(&PShader, &PS);
}