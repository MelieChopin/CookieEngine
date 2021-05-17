#include "ParticlesPass.hpp"
#include "Core/Math/Mat4.hpp"
#include "Render/RendererRemote.hpp"
#include "Resources/Mesh.hpp"
#include "camera.hpp"

using namespace Cookie;
using namespace Cookie::Render;

struct VS_CONSTANT_BUFFER
{
    Cookie::Core::Math::Mat4 proj = Cookie::Core::Math::Mat4::Identity();
    Cookie::Core::Math::Mat4 view = Cookie::Core::Math::Mat4::Identity();
    Cookie::Core::Math::Vec3 pos;
    float padding;
};


ParticlesPass::ParticlesPass()
{
    InitShader();
}

ParticlesPass::~ParticlesPass()
{
    if (VShader)
        VShader->Release();
    if (PShader)
        PShader->Release();
    if (CBuffer)
        CBuffer->Release();
    if (ILayout)
        ILayout->Release();
    if (InstanceBuffer)
        InstanceBuffer->Release();
}

void ParticlesPass::InitShader()
{
	ID3DBlob* blob = nullptr;

	std::string source = (const char*)R"(#line 28
    cbuffer MatrixBuffer : register(b0)
    {
        float4x4 gProj;
	    float4x4 gView;
        float3 gCamPos;
        float padding;
    };

    struct VertexInputType
    {
        float3 PosL     : POSITION;
	    float2 Tex      : TEXCOORD;
        float3 NormalL  : NORMAL;
        float4x4 World  : WORLD;
	    float4 Color    : COLOR;
	    uint InstanceId : SV_InstanceID;
    };
    
    struct PixelInputType
    {
        float4 PosH    : SV_POSITION;
        float3 PosW    : POSITION;
        float3 NormalW : NORMAL;
	    float2 Tex     : TEXCOORD;
	    float4 Color   : COLOR;
    };
    
    PixelInputType main(VertexInputType vin)
    {
    	PixelInputType vout;

        float4 temp = mul(float4(vin.PosL, 1.0f), vin.World);

        float3 forward = float3(0, 0, 1);
        float3 cameraObj = normalize(gCamPos - temp);
        float angle = 1;
        if (dot(float3(-1, 0, 0), cameraObj) > 0)
            angle = 3.1415 * 2 - acos(dot(normalize(forward), normalize(cameraObj)));
        else
            angle = acos(dot(normalize(forward), normalize(cameraObj)));

        float4x4 rot = float4x4(float4(cos(angle), 0, -sin(angle), 0),
                                float4(0, 1, 0, 0),
                                float4(sin(angle), 0, cos(angle), 0),
                                float4(0, 0, 0, 1));

        temp = mul(float4(vin.PosL, 1.0f), mul(rot, vin.World));

	    // Transform to world space space.
	    vout.PosW    = temp.xyz;
	    vout.NormalW = mul(vin.NormalL, (float3x3)vin.World);
        
        float4x4 mat = mul(gView, gProj);

	    // Transform to homogeneous clip space.
	    vout.PosH = mul(temp, mat);
	    
	    // Output vertex attributes for interpolation across triangle.
	    vout.Tex   = mul(float4(vin.Tex, 0.0f, 1.0f), vin.World).xy;
	    vout.Color = vin.Color;

	    return vout;
    }
	)";

	Render::CompileVertex(source, &blob, &VShader);

	source = (const char*)R"(#line 53

    struct PixelInputType
    {
        float4 PosH    : SV_POSITION;
        float3 PosW    : POSITION;
        float3 NormalW : NORMAL;
	    float2 Tex     : TEXCOORD;
	    float4 Color   : COLOR;
    };

    float4 main(PixelInputType input) : SV_TARGET
    {
        return input.Color;
    }
	)";

	Render::CompilePixel(source, &PShader);

    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64,  D3D11_INPUT_PER_INSTANCE_DATA, 1 }
    };
     
    Render::CreateLayout(&blob, ied, 8, &ILayout);

    mInstancedData.resize(1);

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(InstancedData) * mInstancedData.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    Render::RendererRemote::device->CreateBuffer(&vbd, 0, &InstanceBuffer);

    VS_CONSTANT_BUFFER buffer = {};
    buffer.pos = Cookie::Core::Math::Vec3();
    buffer.proj = Cookie::Core::Math::Mat4::Identity();
    buffer.view = Cookie::Core::Math::Mat4::Identity();

    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(VS_CONSTANT_BUFFER);
    vbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &buffer;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    Render::RendererRemote::device->CreateBuffer(&vbd, &InitData, &CBuffer);

    D3D11_BLEND_DESC blenDesc = {  };

    blenDesc.AlphaToCoverageEnable = false;
    blenDesc.IndependentBlendEnable = false;
    blenDesc.RenderTarget[0].BlendEnable = true;
    blenDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blenDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO; 
    blenDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blenDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blenDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blenDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    RendererRemote::device->CreateBlendState(&blenDesc, &blendState);
}

/*=========================== REALTIME METHODS ===========================*/

void ParticlesPass::AllocateMoreSpace(int newSpace)
{
    InstanceBuffer->Release();
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.ByteWidth = sizeof(InstancedData) * newSpace;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.MiscFlags = 0;
    vbd.StructureByteStride = 0;
    Render::RendererRemote::device->CreateBuffer(&vbd, 0, &InstanceBuffer);
    mInstancedData.resize(newSpace);
}

void ParticlesPass::Draw(const Cookie::Render::Camera& cam, Resources::Mesh* mesh, std::vector<InstancedData> data)
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);

    const float blendFactor[4] = { 1.0f,1.0f,1.0f, 1.0f };
    Render::RendererRemote::context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

    if (mInstancedData.size() < data.size())
        AllocateMoreSpace(data.size());

    mInstancedData = data;

    ID3D11Buffer* vbs[2] = { mesh->VBuffer, InstanceBuffer };

    UINT stride[2] = { sizeof(Cookie::Core::Math::Vec3) + sizeof(Cookie::Core::Math::Vec2) + sizeof(Cookie::Core::Math::Vec3), sizeof(InstancedData) };
    UINT offset[2] = { 0, 0 };

    Render::RendererRemote::context->IASetInputLayout(ILayout);
    Render::RendererRemote::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);

    VS_CONSTANT_BUFFER buffer = {};
    buffer.proj = cam.GetProj();
    buffer.view = cam.GetView();
    buffer.pos = cam.pos;
    Render::WriteCBuffer(&buffer, sizeof(buffer), 0, &CBuffer);
 
    Render::RendererRemote::context->IASetVertexBuffers(0, 2, vbs, stride, offset);
    Render::RendererRemote::context->IASetIndexBuffer(mesh->IBuffer, DXGI_FORMAT_R32_UINT, 0);

    D3D11_MAPPED_SUBRESOURCE ms;
    HRESULT result = Render::RendererRemote::context->Map(InstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);


    /*for (int i = 0; i < mInstancedData.size(); i++)
    {
        if ()
        
    }*/

    memcpy(ms.pData, mInstancedData.data(), sizeof(Render::InstancedData) * mInstancedData.size());
    Render::RendererRemote::context->Unmap(InstanceBuffer, 0);

    Render::RendererRemote::context->DrawIndexedInstanced(6, mInstancedData.size(), 0, 0, 0);
}