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

constexpr float upEpsilon = 1.f;

struct VS_CONSTANT_BUFFER
{
    Mat4 model;
    Mat4 viewProj;
};


/*=========================== CONSTRUCTORS/DESTRUCTORS ===========================*/

SelectionDrawer::SelectionDrawer()
{
    /* creating a 2D arrow  on x/z plane */
    std::vector<float> vertices = { -0.10f, upEpsilon, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.10f, upEpsilon, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.10f, upEpsilon,  0.25f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                    -0.10f, upEpsilon,  0.25f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
                                    -0.25f, upEpsilon, 0.25f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
                                     0.25f, upEpsilon, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
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

    selectedMatrices = &drawData.selectedMatrices;
    selectedModels = &drawData.selectedModels;

    for (int i = 0; drawData.selectedGameplays.size(); i++)
    {
        ECS::ComponentGameplay iGameplay = drawData.selectedGameplays[i];
        Mat4 mat = selectedMatrices->at(i);
        if (iGameplay.signatureGameplay & ECS::CGP_SIGNATURE::PRODUCER)
        {
            arrowsMatrices.push_back(Mat4::Translate( (iGameplay.componentProducer.newUnitDestination + Vec3(mat.e[4], mat.e[7], mat.e[10]))*0.5f) );
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
    
    //drawing arrows
    arrowMesh->Set();

    Vec3 dest = { 7.5f,0.0f,6.5f };

    //for (int i = 0; i < arrowsMatrices.size(); i++)
    //{
        buffer.model = Mat4::Scale({ (dest.x) * 2.0f,1.0f,(dest.z) * 2.0f }) * Mat4::Translate(dest*0.5f);
        Render::WriteCBuffer(&buffer, bufferSize, 0, &VCBuffer);
        arrowMesh->Draw();
    //}

}

void SelectionDrawer::Clear()
{
    arrowsMatrices.clear();
}