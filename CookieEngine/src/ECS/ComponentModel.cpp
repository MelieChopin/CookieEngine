#include "Render/D3D11Helper.hpp"
#include "Mat4.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "ECS/ComponentModel.hpp"

using namespace Cookie::ECS;

struct VS_CONSTANT_BUFFER
{
    Cookie::Core::Math::Mat4 viewproj = Cookie::Core::Math::Mat4::Identity();
    Cookie::Core::Math::Mat4 model = Cookie::Core::Math::Mat4::Identity();
};

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
    texture = nullptr;
}

void ComponentModel::Draw(const Core::Math::Mat4& viewProj, const Core::Math::Mat4& modelMat, ID3D11Buffer** CBuffer)
{
    VS_CONSTANT_BUFFER buffer = { viewProj, modelMat };

    Render::WriteCBuffer(&buffer, sizeof(buffer), 0, CBuffer);

    if (texture)
        texture->Set();
    if (mesh)
    {
        mesh->Set();
        mesh->Draw();
    }
}