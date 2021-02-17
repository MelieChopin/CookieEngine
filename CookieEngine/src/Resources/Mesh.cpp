#include <d3d11.h>
#include "Render/Renderer.hpp"
#include <vector>
#include "Resources/Mesh.hpp"

using namespace Cookie::Resources;

Mesh::Mesh(aiMesh* mesh, Render::Renderer& renderer)
{
    name = mesh->mName.C_Str();
    InitVBuffer(mesh, renderer);
    InitIBuffer(mesh,renderer);
}

Mesh::~Mesh()
{
	VBuffer->Release();
}

void Mesh::InitVBuffer(aiMesh* mesh, Render::Renderer& renderer)
{
    D3D11_BUFFER_DESC bDesc = {};

    std::vector<float> vertices;
    vertices.reserve(mesh->mNumVertices * 3 * 3 * 2);

    for (int i = 0; i <= mesh->mNumVertices; i++)
    {
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        vertices.push_back(mesh->mTextureCoords[i]->x);
        vertices.push_back(mesh->mTextureCoords[i]->y);
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
    indices.reserve(mesh->mNumFaces * 3);

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