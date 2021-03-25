#include "Physics/PhysicsHandle.hpp"
#include "Render/DebugRenderer.hpp"

using namespace Cookie::Render;

/*========== CONSTRUCTORS/DESTRUCTORS ==========*/

DebugRenderer::DebugRenderer()
{
}

DebugRenderer::~DebugRenderer()
{
}

/*========== RUNTIME METHODS ==========*/

void DebugRenderer::SetPhysicsRendering()
{
    Physics::PhysicsHandle physHandle;
	if (showDebug)
	{
        physHandle.physSim->setIsDebugRenderingEnabled(true);
        rp3d::DebugRenderer& physicsDebug = physHandle.physSim->getDebugRenderer();
        physicsDebug.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
        physicsDebug.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
        physicsDebug.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
        physicsDebug.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
        physicsDebug.setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
	}
    else
    {
        physHandle.physSim->setIsDebugRenderingEnabled(false);
    }
}

void DebugRenderer::Draw()
{
    if (showDebug)
    {

    }
}