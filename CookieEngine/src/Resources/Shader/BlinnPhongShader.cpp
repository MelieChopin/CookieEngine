#include <d3d11.h>
#include <d3dcompiler.h>
#include <system_error>

#include "reactphysics3d/reactphysics3d.h"
#include "Vec4.hpp"
#include "Mat4.hpp"
#include "Render/Renderer.hpp"
#include "Render/RendererRemote.hpp"
#include "Resources/Shader/BlinnPhongShader.hpp"

using namespace Cookie::Resources;

struct VS_CONSTANT_BUFFER
{
    Cookie::Core::Math::Mat4 proj = Cookie::Core::Math::Mat4::Identity();
    Cookie::Core::Math::Mat4 view = Cookie::Core::Math::Mat4::Identity();
};

BlinnPhongShader::BlinnPhongShader(std::string _name) :
    Shader(_name)
{
    ID3DBlob* VS;

    if (CompileVertex(&VS) && CompilePixel())
        if (CreateLayout(&VS) && CreateSampler())
            CreateBuffer();
}

BlinnPhongShader::~BlinnPhongShader()
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

std::string BlinnPhongShader::GetVertexSource()
{
    return (const char*)R"(struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV; 
        float3 normal : NORMAL;
    };
    
    cbuffer VS_CONSTANT_BUFFER : register(b0)
    {
        float4x4  viewProj;
        float4x4  model;
    };
    
    VOut main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL)
    {
        VOut output;
    
        output.position = mul(mul(float4(position,1.0),model), viewProj);
        output.uv       = uv;
        output.normal   = normal;
    
        return output;

    }
    )";
}

std::string BlinnPhongShader::GetPixelSource()
{
    return (const char*)R"(

    Texture2D	diffuseTex2D : register(t0);    

    SamplerState WrapSampler : register(s0);

    #define MAX_DIR_LIGHTS     10
    #define MAX_POINT_LIGHTS   10
    #define MAX_SPOT_LIGHTS    10

    struct PointLight
    {
        float3 pos;
		float3 color;

		float radius = 0.0;
    };

    cbuffer DirLights : register(b1)
    {
        float3 dir  [MAX_DIR_LIGHTS];
        float3 color[MAX_DIR_LIGHTS];
    };

    cbuffer SphereLights : register(b2)
    {
        float3  pos     [MAX_POINT_LIGHTS];
        float3  color   [MAX_POINT_LIGHTS];
        float   radius  [MAX_POINT_LIGHTS];
    };

    cbuffer SpotLights : register(b3)
    {
        float3  pos     [MAX_SPOT_LIGHTS];
        float3  dir     [MAX_SPOT_LIGHTS];
        float3  color   [MAX_SPOT_LIGHTS];

        float   radius  [MAX_SPOT_LIGHTS];
        float   angle   [MAX_SPOT_LIGHTS];
    };

    float4 main(float4 position : SV_POSITION, float2 uv : UV, float3 normal : NORMAL) : SV_TARGET
    {
        return diffuseTex2D.Sample(WrapSampler,uv);
    })";
}


bool BlinnPhongShader::CompileVertex(ID3DBlob** VS)
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

bool BlinnPhongShader::CompilePixel()
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

bool BlinnPhongShader::CreateLayout(ID3DBlob** VS)
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

    HRESULT result = Render::RendererRemote::device->CreateInputLayout(ied, 3, (*VS)->GetBufferPointer(), (*VS)->GetBufferSize(), &layout);

    if (FAILED(result))
    {

        printf("FAILED to create input layout: %s \n", std::system_category().message(result).c_str());
        return false;
    }

    return true;
}

bool BlinnPhongShader::CreateBuffer()
{
    D3D11_BUFFER_DESC bDesc = {};

    bDesc.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
    bDesc.Usage = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags = 0;
    bDesc.StructureByteStride = 0;

    VS_CONSTANT_BUFFER buffer = {};

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &buffer;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    if (FAILED(Render::RendererRemote::device->CreateBuffer(&bDesc, &InitData, &CBuffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", CBuffer, sizeof(InitData.pSysMem));
        return false;
    }

    return true;
}

bool BlinnPhongShader::CreateSampler()
{
    // Create a sampler state
    D3D11_SAMPLER_DESC SamDesc = {};
    SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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

void BlinnPhongShader::Set(const Core::Math::Mat4& proj, const Core::Math::Mat4& view)
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    D3D11_MAPPED_SUBRESOURCE ms;

    if (FAILED(Render::RendererRemote::context->Map(CBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)))
    {
        printf("Failed to update Buffer nb %u: %p\n", 0, CBuffer);
        return;
    }

    VS_CONSTANT_BUFFER vcb = { proj,view };

    memcpy(ms.pData, &vcb, sizeof(vcb));

    Render::RendererRemote::context->Unmap(CBuffer, 0);

    Render::RendererRemote::context->IASetInputLayout(layout);
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);
    Render::RendererRemote::context->PSSetSamplers(0, 1, &sampler);
}