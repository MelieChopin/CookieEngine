#include <vector>
#include "D3D11Helper.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Prefab.hpp"
#include "Render/Camera.hpp"
#include "DrawDataHandler.hpp"
#include "PlayerData.hpp"
#include "Render/Drawers/PlayerDrawer.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

struct VS_CONSTANT_BUFFER
{
    Mat4 model;
    Mat4 viewProj;
};

constexpr float upEpsilon = 0.1f;

/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

PlayerDrawer::PlayerDrawer()
{
	InitShader();
}

PlayerDrawer::~PlayerDrawer()
{
    if (VShader)
        VShader->Release();
    if (PShader)
        PShader->Release();
    if (VCBuffer)
        VCBuffer->Release();
    if (PCBuffer)
        PCBuffer->Release();
}

/*=========================== INIT METHODS ===========================*/

void PlayerDrawer::InitShader()
{
	ID3DBlob* blob = nullptr;

    std::string source = (const char*)R"(#line 31
    struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV;
    };

    cbuffer VS_CONSTANT_BUFFER : register(b0)
    {
        float4x4  model;
        float4x4  viewProj;
    };
    
    VOut main(float3 position : POSITION, float2 uv : UV, float3 normal : NORMAL)
    {
        VOut output;
    
        output.position = mul(mul(float4(position,1.0),model), viewProj);
        output.uv       = uv;
    
        return output;

    }
    )";

    Render::CompileVertex(source, &blob, &VShader);

    source = (const char*)R"(#line 31

    Texture2D	albedoTex : register(t0);
    
    SamplerState WrapSampler : register(s0);

    struct VOut
    {
        float4 position : SV_POSITION;
        float2 uv : UV;
    };

    cbuffer PS_CONSTANT_BUFFER : register(b0)
    {
        float4 color;
    };
    
    float4 main(VOut vertexOutput) : SV_TARGET
    {
        float3 texColor     = albedoTex.Sample(WrapSampler,vertexOutput.uv).rgb;

        float4 finalColor   = float4(lerp(texColor*color.rgb,color.rgb,step(0.0,dot(texColor,texColor))),color.a);
    
        return finalColor;

    }
    )";

    Render::CompilePixel(source, &PShader);

    VS_CONSTANT_BUFFER buffer = {};
    Render::CreateBuffer(&buffer, sizeof(VS_CONSTANT_BUFFER), &VCBuffer);

    Vec4 aColor;
    Render::CreateBuffer(&aColor, sizeof(Vec4), &PCBuffer);

    /* creating a quad already rotated */
    std::vector<float> vertices = { -0.5f, upEpsilon, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.5f, upEpsilon, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.5f, upEpsilon,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                    -0.5f, upEpsilon,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f };

    std::vector<unsigned int> indices = { 3, 0, 1, 3, 1, 2 };


    quadMesh = std::make_unique<Resources::Mesh>("SelectQuad", vertices, indices, 6);

    blob->Release();
}

/*=========================== REALTIME METHODS ===========================*/

void PlayerDrawer::Set(const DrawDataHandler& drawData)
{
    const Gameplay::PlayerData& player = *drawData.player;

    playerDrawInfo.viewProj = drawData.currentCam->GetViewProj();

    playerDrawInfo.isMakingQuad = player.makingASelectionQuad;
    if (playerDrawInfo.isMakingQuad)
    {
        Vec3 quadCenter = (player.mousePosInWorld + player.selectionQuadStart) * 0.5f;
        playerDrawInfo.quadTrs = Mat4::Scale({(quadCenter.x - player.selectionQuadStart.x)*2.0f,1.0f,(quadCenter.z - player.selectionQuadStart.z)*2.0f }) * Mat4::Translate(quadCenter);
    }

    playerDrawInfo.isValid = player.isBuildingValid;
    if (player.workerWhoBuild)
    {
        if (player.workerWhoBuild->possibleBuildings[player.indexOfBuildingInWorker])
        {
            playerDrawInfo.buildingMesh         = player.workerWhoBuild->possibleBuildings[player.indexOfBuildingInWorker]->model.mesh;
            playerDrawInfo.buildingAlbedoTex    = player.workerWhoBuild->possibleBuildings[player.indexOfBuildingInWorker]->model.albedo;
            playerDrawInfo.buildingTRS          = player.workerWhoBuild->possibleBuildings[player.indexOfBuildingInWorker]->transform.TRS;
        }
    }
}

void PlayerDrawer::Draw()
{
    Render::RendererRemote::context->VSSetShader(VShader, nullptr, 0);
    Render::RendererRemote::context->PSSetShader(PShader, nullptr, 0);
    Render::RendererRemote::context->VSSetConstantBuffers(0, 1, &VCBuffer);
    Render::RendererRemote::context->PSSetConstantBuffers(0, 1, &PCBuffer);

    ID3D11ShaderResourceView* tex[1] = {nullptr};
    Render::RendererRemote::context->PSSetShaderResources(0, 1, tex);

    VS_CONSTANT_BUFFER buffer = {};
    size_t bufferSize = sizeof(VS_CONSTANT_BUFFER);

    buffer.viewProj = playerDrawInfo.viewProj;

    Vec4 color = playerDrawInfo.validColor;
    size_t colorSize = sizeof(Vec4);

    if (playerDrawInfo.isMakingQuad)
    {
        buffer.model = playerDrawInfo.quadTrs;
        Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);

        Render::WriteCBuffer(&color, colorSize, 0, &PCBuffer);

        quadMesh->Set();
        quadMesh->Draw();
    }

    if (playerDrawInfo.buildingMesh)
    {
        buffer.model = playerDrawInfo.buildingTRS;
        Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);

        if (!playerDrawInfo.isValid)
        {
            color = playerDrawInfo.invalidColor;
            Render::WriteCBuffer(&color, colorSize, 0, &PCBuffer);
        }

        if (playerDrawInfo.buildingAlbedoTex)
        {
            tex[0] = playerDrawInfo.buildingAlbedoTex->GetResourceView();
            Render::RendererRemote::context->PSSetShaderResources(0, 1, tex);
        }

        playerDrawInfo.buildingMesh->Set();
        playerDrawInfo.buildingMesh->Draw();
    }
}