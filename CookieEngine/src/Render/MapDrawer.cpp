#include <D3D11Helper.hpp>
#include "Render/RendererRemote.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Map.hpp"
#include "Primitives.hpp"
#include "Render/MapDrawer.hpp"

using namespace Cookie::Render;
using namespace Cookie::Core::Math;

struct VS_CONSTANT_BUFFER
{
    Mat4 model;
    Vec4 tileSize;
};

/*============================= CONSTRUCTORS/DESTRUCTORS =============================*/

MapDrawer::MapDrawer():
    mapInfo{ Core::Primitives::CreateCube() }
{
	InitShader();
}

MapDrawer::~MapDrawer()
{
	if (VShader)
		VShader->Release();
	if (CBuffer)
		CBuffer->Release();
}

/*============================= INIT METHODS =============================*/

void MapDrawer::InitShader()
{
    ID3DBlob* blob = nullptr;

    std::string source = (const char*)R"(#line 27
    struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV; 
        float3 normal : NORMAL;
        float3 fragPos : FRAGPOS;
        float4 view    : VIEW;
    };
    
    cbuffer MODEL_CONSTANT : register(b0)
    {
        float4x4  model;
        float2    tileSize;
    };

    cbuffer CAM_CONSTANT : register(b1)
    {
        float4x4  proj;
        float4x4  view;
    };

    
    VOut main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL)
    {
        VOut output;
    
        output.fragPos  = mul(float4(position,1.0),model).xyz;
        output.view     = mul(float4(output.fragPos,1.0),view);
        output.position = mul(output.view, proj);
        output.uv       = uv * tileSize;
        output.normal   = normalize(mul(normal,(float3x3)model));

    
        return output;

    }
    )";

    Render::CompileVertex(source, &blob, &VShader);

    VS_CONSTANT_BUFFER buffer = {};

    Render::CreateBuffer(&buffer, sizeof(VS_CONSTANT_BUFFER), &CBuffer);

}

/*============================= REALTIME METHODS =============================*/

void MapDrawer::Set(const Resources::Map& map)
{
    mapInfo.albedoTex = map.model.albedo;
    mapInfo.normalTex = map.model.normal;
    mapInfo.matTex = map.model.metallicRoughness;

    mapInfo.model = map.trs.TRS;
    mapInfo.tileSize = map.tilesSize;
}

void MapDrawer::Draw()
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    VS_CONSTANT_BUFFER buffer   = {};
    buffer.model                = mapInfo.model;
    buffer.tileSize             = { mapInfo.tileSize.x,mapInfo.tileSize.y,0.0f,0.0f };

    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &CBuffer);
    Render::WriteCBuffer(&buffer, sizeof(buffer), 0, &CBuffer);

    if (mapInfo.albedoTex)
        mapInfo.albedoTex->Set(0);
    if (mapInfo.normalTex)
        mapInfo.normalTex->Set(1);
    if (mapInfo.matTex)
        mapInfo.matTex->Set(2);

    mapInfo.mapMesh->Set();
    mapInfo.mapMesh->Draw();
}