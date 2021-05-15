#include "ECS/ComponentHandler.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Resources/Prefab.hpp"

using namespace Cookie::ECS;

/*============ PHYSICS ============*/

void ComponentHandler::InitComponentPhysic(Entity& entity)
{
	if (entity.signature & SIGNATURE_TRANSFORM)
		componentPhysics[entity.id].Set(componentTransforms[entity.id]);

	componentPhysics[entity.id].physBody = Physics::PhysicsHandle::physSim->createRigidBody(componentPhysics[entity.id].physTransform);
}
