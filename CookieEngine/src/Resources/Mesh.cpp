#include <d3d11.h>
#include "Render/RendererRemote.hpp"
#include "Render/Renderer.hpp"
#include <vector>
#include "Physics/PhysicsHandle.hpp"
#include "Resources/Mesh.hpp"

using namespace Cookie::Resources;

Mesh::Mesh(std::string _name, aiMesh* mesh):
    name{_name}
{
    INb     = mesh->mNumFaces * 3;//a face is a triangle as triangulate flag is enabled
    InitVBuffer(mesh);
    InitIBuffer(mesh);

    aiVector3D center   = (mesh->mAABB.mMax + mesh->mAABB.mMin) * 0.5f;
    AABBhalfExtent      = {mesh->mAABB.mMax.x - center.x,mesh->mAABB.mMax.y - center.y,mesh->mAABB.mMax.z - center.z};
}

Mesh::Mesh(std::string meshName, std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int inb):
    name{meshName}
{
    INb = inb;
    InitVBuffer(vertices);
    InitIBuffer(indices);
    ComputeAABB(vertices);
}

Mesh::~Mesh()
{
    if (VBuffer)
	    VBuffer->Release();
    if (IBuffer)
        IBuffer->Release();
}

void Mesh::ComputeAABB(const std::vector<float>& vertices)
{
    for (int i = 0; i < vertices.size()/3; i++)
    {
        if (std::abs(vertices[i]) > AABBhalfExtent.x)
        {
            AABBhalfExtent.x = vertices[i];
        }
        if (std::abs(vertices[i+1]) > AABBhalfExtent.y)
        {
            AABBhalfExtent.y = vertices[i+1];
        }
        if (std::abs(vertices[i+2]) > AABBhalfExtent.z)
        {
            AABBhalfExtent.z = vertices[i+2];
        }
    }
}

void Mesh::InitVBuffer(aiMesh* mesh)
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

    if (FAILED(Render::RendererRemote::device->CreateBuffer(&bDesc, &InitData, &VBuffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", VBuffer, sizeof(InitData.pSysMem));
    }
}

void Mesh::InitIBuffer(aiMesh* mesh)
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

    if (FAILED(Render::RendererRemote::device->CreateBuffer(&bDesc, &InitData, &IBuffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", IBuffer, sizeof(InitData.pSysMem));
    }
}

void Mesh::InitVBuffer(std::vector<float>& vertices)
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

    if (FAILED(Render::RendererRemote::device->CreateBuffer(&bDesc, &InitData, &VBuffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", VBuffer, sizeof(InitData.pSysMem));
    }
}

void Mesh::InitIBuffer(std::vector<unsigned int>& indices)
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

    if (FAILED(Render::RendererRemote::device->CreateBuffer(&bDesc, &InitData, &IBuffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", IBuffer, sizeof(InitData.pSysMem));
    }
}

void Mesh::Set()
{
    UINT stride = ((2 * sizeof(Core::Math::Vec3)) + sizeof(Core::Math::Vec2));
    UINT offset = 0;
    Render::RendererRemote::context->IASetVertexBuffers(0,1,&VBuffer,&stride,&offset);
    Render::RendererRemote::context->IASetIndexBuffer(IBuffer, DXGI_FORMAT_R32_UINT,0);
    Render::RendererRemote::context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::Draw()
{
    Render::RendererRemote::context->DrawIndexed(INb, 0, 0);
}