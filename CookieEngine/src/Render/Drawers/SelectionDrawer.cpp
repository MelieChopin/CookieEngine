#include <vector>
#include "D3D11Helper.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Prefab.hpp"
#include "Render/Camera.hpp"
#include "DrawDataHandler.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Render/Drawers/SelectionDrawer.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

constexpr float upEpsilon = 0.5f;

struct VS_CONSTANT_BUFFER
{
    Mat4 model;
    Mat4 viewProj;
};


/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

SelectionDrawer::SelectionDrawer()
{
    /* creating a 2D arrow  on x/z plane */
    std::vector<float> vertices = { -0.05f, upEpsilon, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.05f, upEpsilon, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.05f, upEpsilon,  0.45f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                    -0.05f, upEpsilon,  0.45f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                    -0.15f, upEpsilon, 0.45f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.15f, upEpsilon, 0.45f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                     0.0f, upEpsilon, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f};

    std::vector<unsigned int> indices = { 3, 0, 1, 3, 1, 2, 4 , 5 , 6 };


    arrowMesh = std::make_unique<Resources::Mesh>("Arrow", vertices, indices, 9);
}

SelectionDrawer::~SelectionDrawer()
{
}

/*=========================== INIT METHODS ===========================*/


/*=========================== REALTIME METHODS ===========================*/

void SelectionDrawer::Set(const DrawDataHandler& drawData)
{
    viewProj = drawData.currentCam->GetViewProj();
    stencilBuffer = drawData.depthStencilView;
    selectedMatrices = &drawData.selectedMatrices;
    selectedModels = &drawData.selectedModels;

    for (int i = 0; i < drawData.selectedGameplays.size(); i++)
    {
        ECS::ComponentGameplay iGameplay = drawData.selectedGameplays[i];
       
        if (iGameplay.signatureGameplay & ECS::CGP_SIGNATURE::PRODUCER)
        {
            Mat4 mat = selectedMatrices->at(i);
            Vec3 pos = Vec3(mat.e[3], mat.e[7], mat.e[11]);
            Vec3 tan = (iGameplay.componentProducer.newUnitDestination - pos);

           
            Mat4 arrMat = Mat4::Scale({ 5.0f ,1.0f, tan.Length()});
            arrMat = arrMat * Mat4::LookAt({ 0.0f,0.0f,0.0f }, Vec3(tan.x,0.0f,-tan.z).Normalize(), {0.0f,1.0f,0.0f});
            arrMat = arrMat * Mat4::Translate(pos + (tan * 0.5f));

            arrowsMatrices.push_back(arrMat);
        }
    }
}

void SelectionDrawer::FillStencil(ID3D11Buffer* VCBuffer)
{
    VS_CONSTANT_BUFFER buffer = {};
    buffer.viewProj = viewProj;
    size_t bufferSize = sizeof(VS_CONSTANT_BUFFER);

    const std::vector<ECS::ComponentModel> _selectedModels = *selectedModels;
    const std::vector<Mat4> _selectedMatrices = *selectedMatrices;

    for (int i = 0; i < _selectedModels.size(); i++)
    {
        ECS::ComponentModel model = _selectedModels[i];

        if (model.mesh)
        {
            buffer.model = _selectedMatrices[i];
            Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);

            model.mesh->Set();
            model.mesh->Draw();
        }
    }
}

void SelectionDrawer::Draw(ID3D11Buffer* VCBuffer, ID3D11Buffer* PCBuffer)
{
    ID3D11ShaderResourceView* tex[1] = { nullptr };
    Render::RendererRemote::context->PSSetShaderResources(0, 1, tex);

    VS_CONSTANT_BUFFER buffer = {};
    buffer.viewProj = viewProj;

    Vec4 color = SELECTION_COLOR;
    WriteCBuffer(&color, sizeof(Vec4), 0, &PCBuffer);

    size_t bufferSize = sizeof(VS_CONSTANT_BUFFER);

    // drawing outLines
    const std::vector<ECS::ComponentModel> _selectedModels = *selectedModels;
    const std::vector<Mat4> _selectedMatrices = *selectedMatrices;

    for (int i = 0; i < _selectedModels.size(); i++)
    {
        ECS::ComponentModel model = _selectedModels[i];

        if (model.mesh)
        {
            buffer.model = outLineSize * _selectedMatrices[i];
            Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);

            model.mesh->Set();
            model.mesh->Draw();
        }
    }

    /* clear stencil */
    Render::RendererRemote::context->ClearDepthStencilView(stencilBuffer, D3D11_CLEAR_STENCIL, 1.0f, 0);

    //drawing arrows
    arrowMesh->Set();

    Vec3 src = { -1.0f,0.0f,0.0f };
    Vec3 dest = { 7.5f,0.0f,6.5f };

    for (int i = 0; i < arrowsMatrices.size(); i++)
    {
        buffer.model = arrowsMatrices[i];
        Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);
        arrowMesh->Draw();
    }
}

void SelectionDrawer::Clear()
{
    arrowsMatrices.clear();
}