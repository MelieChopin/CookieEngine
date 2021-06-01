#include "Core/Math/Mat4.hpp"
#include "Render/D3D11Helper.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Render/DrawDataHandler.hpp"
#include "Render/Drawers/MiniMapDrawer.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

MiniMapDrawer::MiniMapDrawer()
{
    AllocateInstance(1);
}

MiniMapDrawer::~MiniMapDrawer()
{
    if (IMatricesBuffer)
        IMatricesBuffer->Release();
    if (IArmyBuffer)
        IArmyBuffer->Release();
}

/*======================= REALTIME METHODS =======================*/

void MiniMapDrawer::AllocateInstance(unsigned int size)
{
    if (IMatricesBuffer)
        IMatricesBuffer->Release();

    IBufferSize = size;

    D3D11_BUFFER_DESC bDesc = {};

    bDesc.ByteWidth = IBufferSize * sizeof(Mat4);
    bDesc.Usage             = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags         = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags    = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags = 0;
    bDesc.StructureByteStride = 0;


    HRESULT result = Render::RendererRemote::device->CreateBuffer(&bDesc, nullptr, &IMatricesBuffer);

    if (FAILED(result))
    {
        printf("Failed Creating Buffer: %p of size %u: %s \n", IMatricesBuffer, bDesc.ByteWidth, std::system_category().message(result).c_str());
    }

    bDesc.ByteWidth = IBufferSize * sizeof(float);

    result = Render::RendererRemote::device->CreateBuffer(&bDesc, nullptr, &IArmyBuffer);

    if (FAILED(result))
    {
        printf("Failed Creating Buffer: %p of size %u: %s \n", IArmyBuffer, bDesc.ByteWidth, std::system_category().message(result).c_str());
    }
}

void MiniMapDrawer::Draw(const std::vector<DrawData>& toDraw)
{
    ID3D11ShaderResourceView* fbos[3] = { nullptr, nullptr, nullptr };

    Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);

    for (int i = 0; i < toDraw.size(); i++)
    {
        const std::vector<Mat4>& mat = toDraw[i].matrices;
        const std::vector<const ECS::ComponentGameplay*>& gameplays = toDraw[i].gameplays;

        if (mat.size() > IBufferSize)
            AllocateInstance(mat.size());

        WriteBuffer(mat.data(), sizeof(Mat4) * mat.size(), 0, &IMatricesBuffer);

        std::vector<float> ids;

        for (int j = 0; j < gameplays.size(); j++)
        {
            ids.push_back(gameplays.at(j)->teamName);
        }

        WriteBuffer(ids.data(), sizeof(float) * gameplays.size(), 0, &IArmyBuffer);

        UINT stride[3] = { ((2 * sizeof(Core::Math::Vec3)) + sizeof(Core::Math::Vec2)), sizeof(Mat4), sizeof(float) };
        UINT offset[3] = { 0 , 0, 0 };
        ID3D11Buffer* VBuffers[3] = { toDraw[i].mesh->VBuffer, IMatricesBuffer, IArmyBuffer };

        RendererRemote::context->IASetIndexBuffer(toDraw[i].mesh->IBuffer, DXGI_FORMAT_R32_UINT, 0);
        RendererRemote::context->IASetVertexBuffers(0, 3, VBuffers, stride, offset);

        RendererRemote::context->DrawIndexedInstanced(toDraw[i].mesh->INb, mat.size(), 0, 0, 0);

        Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);
    }
}