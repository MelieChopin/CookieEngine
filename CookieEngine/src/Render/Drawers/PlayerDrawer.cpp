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

constexpr float upEpsilon = 0.1f;


struct VS_CONSTANT_BUFFER
{
    Mat4 model;
    Mat4 viewProj;
};


/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

PlayerDrawer::PlayerDrawer()
{
    /* creating a quad already rotated */
    std::vector<float> vertices = { -0.5f, upEpsilon, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.5f, upEpsilon, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.5f, upEpsilon,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                    -0.5f, upEpsilon,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f };

    std::vector<unsigned int> indices = { 3, 0, 1, 3, 1, 2 };


    quadMesh = std::make_unique<Resources::Mesh>("SelectQuad", vertices, indices, 6);
}

PlayerDrawer::~PlayerDrawer()
{
}

/*=========================== INIT METHODS ===========================*/


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
            playerDrawInfo.buildingTRS          = player.workerWhoBuild->possibleBuildings[player.indexOfBuildingInWorker]->transform.TRS * Mat4::Translate(player.buildingPos);
        }
    }
    else
    {
        playerDrawInfo.buildingMesh = nullptr;
    }
}

void PlayerDrawer::Draw(ID3D11Buffer* VCBuffer, ID3D11Buffer* PCBuffer)
{
    ID3D11ShaderResourceView* tex[1] = {nullptr};
    Render::RendererRemote::context->PSSetShaderResources(0, 1, tex);

    VS_CONSTANT_BUFFER buffer = {};
    size_t bufferSize = sizeof(VS_CONSTANT_BUFFER);

    buffer.viewProj = playerDrawInfo.viewProj;

    Vec4 color = playerDrawInfo.validColor;
    size_t colorSize = sizeof(Vec4);
    Render::WriteCBuffer(&color, colorSize, 0, &PCBuffer);

    if (playerDrawInfo.isMakingQuad)
    {
        buffer.model = playerDrawInfo.quadTrs;
        Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);

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