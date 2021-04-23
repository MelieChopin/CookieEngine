#include "Render/RendererRemote.hpp"
#include "reactphysics3d/reactphysics3d.h"
#include "Resources/Shader/PhysicsShader.hpp"
#include "Mat4.hpp"

#include <d3dcompiler.h>

using namespace Cookie::Resources;

struct VS_CONSTANT_BUFFER
{
    Cookie::Core::Math::Mat4 viewProj = Cookie::Core::Math::Mat4::Identity();
};

PhysicsShader::PhysicsShader(std::string _name):
    Shader(_name)
{
    ID3DBlob* VS;

    if (CompileVertex(&VS) && CompilePixel())
        if (CreateLayout(&VS))
            CreateBuffer();
}

PhysicsShader::~PhysicsShader()
{
    if (PShader)
        PShader->Release();
    if (VShader)
        VShader->Release();
    if (layout)
        layout->Release();
}

std::string PhysicsShader::GetVertexSource()
{
    return (const char*)R"(struct VOut
    {
        float4 position : SV_POSITION;
        uint  color : COLOR;
    };

    cbuffer VS_CONSTANT_BUFFER : register(b0)
    {
        float4x4  viewProj;
    };
    
    VOut main(float3 position : POSITION, uint color : COLOR)
    {
        VOut output;
    
        output.position = mul(float4(position,1.0), viewProj);
        output.color = color;
    
        return output;

    }
    )";
}

std::string PhysicsShader::GetPixelSource()
{
    return (const char*)R"(
    float4 main(float4 position : SV_POSITION, uint color : COLOR) : SV_TARGET
    {
        float3 finalColor;
        finalColor.r = floor(color / 65536);
        finalColor.g = floor((color - finalColor.r * 65536) / 256.0);
        finalColor.b = floor(color - finalColor.r * 65536 - finalColor.g * 256.0);
        return float4(finalColor,1.0);
    })";
}


bool PhysicsShader::CompileVertex(ID3DBlob** VS)
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

bool PhysicsShader::CompilePixel()
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

bool PhysicsShader::CreateLayout(ID3DBlob** VS)
{
    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(rp3d::DebugRenderer::DebugTriangle,point1), D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32_FLOAT, 0,  offsetof(rp3d::DebugRenderer::DebugTriangle,color1), D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    if (FAILED(Render::RendererRemote::device->CreateInputLayout(ied, 2, (*VS)->GetBufferPointer(), (*VS)->GetBufferSize(), &layout)))
    {
        printf("FAILED to create input layout \n");
        return false;
    }

    return true;
}

bool PhysicsShader::CreateBuffer()
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

void PhysicsShader::Set()
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    Render::RendererRemote::context->IASetInputLayout(layout);
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);

    Render::RendererRemote::currentShader = (Shader*)this;
}

void PhysicsShader::WriteCBuffer(const Core::Math::Mat4& proj, const Core::Math::Mat4& view)
{
    D3D11_MAPPED_SUBRESOURCE ms;

    if (FAILED(Render::RendererRemote::context->Map(CBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)))
    {
        printf("Failed to update Buffer nb %u: %p\n", 0, CBuffer);
        return;
    }

    VS_CONSTANT_BUFFER vcb = { proj };

    memcpy(ms.pData, &vcb, sizeof(vcb));

    Render::RendererRemote::context->Unmap(CBuffer, 0);
}