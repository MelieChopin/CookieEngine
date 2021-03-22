#include <d3d11.h>
#include "Render/RendererRemote.hpp"
#include "Render/Renderer.hpp"
#include <vector>
#include "Resources/Mesh.hpp"

using namespace Cookie::Resources;

Mesh::Mesh(std::string _name, aiMesh* mesh, Render::Renderer& renderer)
{
    name    = _name;
    INb     = mesh->mNumFaces * 3;//a face is a triangle as triangulate flag is enabled
    InitVBuffer(mesh, renderer);
    InitIBuffer(mesh,renderer);
}

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int inb, Render::Renderer& renderer)
{
    INb = inb;
    InitVBuffer(vertices, renderer);
    InitIBuffer(indices, renderer);
}

Mesh::~Mesh()
{
    if (VBuffer)
	    VBuffer->Release();
    if (IBuffer)
        IBuffer->Release();
}

void Mesh::InitVBuffer(aiMesh* mesh, Render::Renderer& renderer)
{
    D3D11_BUFFER_DESC bDesc = {};

    std::vector<float> vertices;

    for (int i = 0; i <= mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        vertices.push_back(mesh->mTextureCoords[0][i].x);
        vertices.push_back(mesh->mTextureCoords[0][i].y);
        vertices.push_back(mesh->mNormals[i].x);
        vertices.push_back(mesh->mNormals[i].y);
        vertices.push_back(mesh->mNormals[i].z);
    }

    bDesc.ByteWidth             = vertices.size() * sizeof(float);
    bDesc.Usage                 = D3D11_USAGE_DEFAULT;
    bDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags        = 0;
    bDesc.MiscFlags             = 0;
    bDesc.StructureByteStride   = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem            = vertices.data();
    InitData.SysMemPitch        = 0;
    InitData.SysMemSlicePitch   = 0;

    renderer.CreateBuffer(bDesc, InitData, &VBuffer);
}

void Mesh::InitIBuffer(aiMesh* mesh, Render::Renderer& renderer)
{
    D3D11_BUFFER_DESC bDesc = {};

    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace iFace = mesh->mFaces[i];
        for (unsigned int j = 0; j < iFace.mNumIndices; j++)
            indices.push_back(iFace.mIndices[j]);
    }

    bDesc.ByteWidth             = indices.size() * sizeof(unsigned int);
    bDesc.Usage                 = D3D11_USAGE_DEFAULT;
    bDesc.BindFlags             = D3D11_BIND_INDEX_BUFFER;
    bDesc.CPUAccessFlags        = 0;
    bDesc.MiscFlags             = 0;
    bDesc.StructureByteStride   = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices.data();
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    renderer.CreateBuffer(bDesc, InitData, &IBuffer);
}

void Mesh::InitVBuffer(std::vector<float> vertices, Render::Renderer& renderer)
{
    D3D11_BUFFER_DESC bDesc = {};

    bDesc.ByteWidth = vertices.size() * sizeof(float);
    bDesc.Usage = D3D11_USAGE_DEFAULT;
    bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags = 0;
    bDesc.MiscFlags = 0;
    bDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices.data();
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    renderer.CreateBuffer(bDesc, InitData, &VBuffer);
}

void Mesh::InitIBuffer(std::vector<unsigned int> indices, Render::Renderer& renderer)
{
    D3D11_BUFFER_DESC bDesc = {};

    bDesc.ByteWidth = indices.size() * sizeof(unsigned int);
    bDesc.Usage = D3D11_USAGE_DEFAULT;
    bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bDesc.CPUAccessFlags = 0;
    bDesc.MiscFlags = 0;
    bDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = indices.data();
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    renderer.CreateBuffer(bDesc, InitData, &IBuffer);
}

void Mesh::Set(Render::RendererRemote& remote)
{
    UINT stride = ((2 * sizeof(Core::Math::Vec3)) + sizeof(Core::Math::Vec2));
    UINT offset = 0;
    remote.context->IASetVertexBuffers(0,1,&VBuffer,&stride,&offset);
    remote.context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT,0);
    remote.context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::Draw(Render::RendererRemote& remote)
{
    remote.context->DrawIndexed(INb, 0, 0);
}