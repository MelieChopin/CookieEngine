#include <d3d11.h>
#include "Render/RendererRemote.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Render/DebugRenderer.hpp"

using namespace Cookie::Render;
using namespace Cookie::Resources;
using namespace Cookie::Core::Math;

/*========== CONSTRUCTORS/DESTRUCTORS ==========*/

DebugRenderer::DebugRenderer():
    physDbgRenderer{Physics::PhysicsHandle::editWorld->getDebugRenderer()}
{

    AllocateVBuffer(1);
    InitRasterizerState();
    physDbgRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    physDbgRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
    physDbgRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    physDbgRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
    physDbgRenderer.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
}

DebugRenderer::~DebugRenderer()
{
    if (VBuffer)
        VBuffer->Release();
    if (rasterState)
        rasterState->Release();
}


/*========== INIT METHODS ==========*/

void DebugRenderer::AllocateVBuffer(size_t vBufferSize)
{
    bDesc.ByteWidth             = vBufferSize * sizeof(float);
    bDesc.Usage                 = D3D11_USAGE_DYNAMIC;
    bDesc.BindFlags             = D3D11_BIND_VERTEX_BUFFER;
    bDesc.CPUAccessFlags        = D3D11_CPU_ACCESS_WRITE;
    bDesc.MiscFlags             = 0;
    bDesc.StructureByteStride   = 0;

    if (FAILED(Render::RendererRemote::device->CreateBuffer(&bDesc, nullptr, &VBuffer)))
    {
        printf("Failed Creating Buffer: %p of size %llu \n", VBuffer, vBufferSize * sizeof(float));
    }
}

void DebugRenderer::InitRasterizerState()
{
    D3D11_RASTERIZER_DESC rasterDesc = {};

    // Setup the raster description which will determine how and what polygons will be drawn.
    rasterDesc.AntialiasedLineEnable    = false;
    rasterDesc.CullMode                 = D3D11_CULL_FRONT;
    rasterDesc.DepthBias                = 0;
    rasterDesc.DepthBiasClamp           = 0.0f;
    rasterDesc.DepthClipEnable          = true;
    rasterDesc.FillMode                 = D3D11_FILL_WIREFRAME;
    rasterDesc.FrontCounterClockwise    = false;
    rasterDesc.MultisampleEnable        = false;
    rasterDesc.ScissorEnable            = false;
    rasterDesc.SlopeScaledDepthBias     = 0.0f;

    Render::RendererRemote::device->CreateRasterizerState(&rasterDesc, &rasterState);;
}

/*========== RUNTIME METHODS ==========*/

void DebugRenderer::UpdateVBuffer(size_t vBufferSize, void* data)
{
    D3D11_MAPPED_SUBRESOURCE ms;

    if (FAILED(Render::RendererRemote::context->Map(VBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms)))
    {
        printf("Failed to update Buffer nb %u: %p\n", 0, VBuffer);
        return;
    }

    memcpy(ms.pData, data, vBufferSize);

    Render::RendererRemote::context->Unmap(VBuffer, 0);
}

void DebugRenderer::Draw(const Mat4& viewProj)
{
    if (showDebug)
    {
        UINT stride = sizeof(Core::Math::Vec3) + sizeof(uint32_t);
        UINT offset = 0;
        physShader.Set(viewProj);

        D3D11_PRIMITIVE_TOPOLOGY topo = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
        Render::RendererRemote::context->IAGetPrimitiveTopology(&topo);

        ID3D11RasterizerState* previousState = nullptr;
        Render::RendererRemote::context->RSGetState(&previousState);
        Render::RendererRemote::context->RSSetState(rasterState);

        physDbgRenderer.reset();
        physDbgRenderer.computeDebugRenderingPrimitives(*Physics::PhysicsHandle::editWorld);

        if (physDbgRenderer.getNbTriangles() > 0)
        {
            Render::RendererRemote::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            if (bDesc.ByteWidth < (physDbgRenderer.getNbTriangles() * sizeof(rp3d::DebugRenderer::DebugTriangle)))
            {
                VBuffer->Release();
                AllocateVBuffer((physDbgRenderer.getNbTriangles() * sizeof(rp3d::DebugRenderer::DebugTriangle)));
            }

            UpdateVBuffer((physDbgRenderer.getNbTriangles() * sizeof(rp3d::DebugRenderer::DebugTriangle)), (void*)physDbgRenderer.getTrianglesArray());

            Render::RendererRemote::context->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);

            Render::RendererRemote::context->Draw(physDbgRenderer.getNbTriangles() * 3, 0);
        }

        if (physDbgRenderer.getNbLines() > 0)
        {
            Render::RendererRemote::context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            if (bDesc.ByteWidth < (physDbgRenderer.getNbLines() * sizeof(rp3d::DebugRenderer::DebugLine)))
            {
                VBuffer->Release();
                AllocateVBuffer((physDbgRenderer.getNbLines() * sizeof(rp3d::DebugRenderer::DebugLine)));
            }

            UpdateVBuffer((physDbgRenderer.getNbLines() * sizeof(rp3d::DebugRenderer::DebugLine)), (void*)physDbgRenderer.getLinesArray());
            Render::RendererRemote::context->IASetVertexBuffers(0, 1, &VBuffer, &stride, &offset);

            Render::RendererRemote::context->Draw(physDbgRenderer.getNbLines() * 2, 0);
        }

        Render::RendererRemote::context->IASetPrimitiveTopology(topo);
        Render::RendererRemote::context->RSSetState(previousState);
    }
}