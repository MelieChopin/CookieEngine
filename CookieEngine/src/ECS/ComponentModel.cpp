#include <d3d11.h>
#include "Render/RendererRemote.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Texture.hpp"
#include "ECS/ComponentModel.hpp"

using namespace Cookie::ECS;

/*============================ CONSTRUCTORS ============================*/

ComponentModel::ComponentModel()
{

}

ComponentModel::~ComponentModel()
{

}

/*============================ REALTIME METHODS ============================*/

void ComponentModel::ToDefault()
{
    mesh = nullptr;
    //shader  = nullptr;
    texture = nullptr;
}

void ComponentModel::Draw(const Core::Math::Mat4& viewProj, const Core::Math::Mat4& modelMat)
{
    if (shader)
        shader->Set(viewProj, modelMat);
    if (texture)
        texture->Set();
    if (mesh)
    {
        mesh->Set();
        mesh->Draw();
    }
}