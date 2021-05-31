#include "Core/Math/Mat4.hpp"
#include "Render/D3D11Helper.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Render/DrawDataHandler.hpp"
#include "Render/Drawers/ModelDrawer.hpp"

using namespace Cookie::Core::Math;
using namespace Cookie::Render;

/*======================= CONSTRUCTORS/DESTRUCTORS =======================*/

ModelDrawer::ModelDrawer()
{
    AllocateInstance(1);
}

ModelDrawer::~ModelDrawer()
{
    if (IBuffer)
        IBuffer->Release();
}

/*======================= REALTIME METHODS =======================*/

void ModelDrawer::AllocateInstance(unsigned int size)
{
    if (IBuffer)
        IBuffer->Release();

    IBufferSize = size;

    D3D11_BUFFER_DESC bDesc = {};

    bDesc.ByteWidth             = IBufferSize * sizeof(Mat4);
    bDesc.Usage                 = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags        = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags             = 0;
    bDesc.StructureByteStride   = 0;


    HRESULT result = Render::RendererRemote::device->CreateBuffer(&bDesc, nullptr, &IBuffer);

    if (FAILED(result))
    {
        printf("Failed Creating Buffer: %p of size %u: %s \n", IBuffer, bDesc.ByteWidth, std::system_category().message(result).c_str());
    }
}

void ModelDrawer::Draw(const std::vector<DrawData>& toDraw, bool drawOccluded)
{
    ID3D11ShaderResourceView* fbos[3] = { nullptr, nullptr, nullptr };

    Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);
    for (int i = 0; i < toDraw.size(); i++)
    {
        const std::vector<Mat4>& mat = drawOccluded ? toDraw[i].matrices : toDraw[i].visibleMatrices;

        if (toDraw[i].albedo)
            toDraw[i].albedo->Set(0);
        if (toDraw[i].normalMap)
            toDraw[i].normalMap->Set(1);
        if (toDraw[i].matMap)
            toDraw[i].matMap->Set(2);

        if (mat.size() > IBufferSize)
            AllocateInstance(mat.size());

        WriteBuffer(mat.data(), sizeof(Mat4) * mat.size() , 0, &IBuffer);

        UINT stride[2] = { ((2 * sizeof(Core::Math::Vec3)) + sizeof(Core::Math::Vec2)), sizeof(Mat4) };
        UINT offset[2] = { 0 , 0 };
        ID3D11Buffer* VBuffers[2] = { toDraw[i].mesh->VBuffer, IBuffer };

        RendererRemote::context->IASetIndexBuffer(toDraw[i].mesh->IBuffer, DXGI_FORMAT_R32_UINT, 0);
        RendererRemote::context->IASetVertexBuffers(0, 2, VBuffers, stride, offset);

        RendererRemote::context->DrawIndexedInstanced(toDraw[i].mesh->INb, mat.size(), 0, 0, 0);

        Render::RendererRemote::context->PSSetShaderResources(0, 3, fbos);
    }
}