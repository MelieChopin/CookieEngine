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
	if (entity.signature & C_SIGNATURE::TRANSFORM)
		componentPhysics[entity.id].Set(componentTransforms[entity.id]);

	componentPhysics[entity.id].physBody = Physics::PhysicsHandle::physSim->createRigidBody(componentPhysics[entity.id].physTransform);
}

/*============ PREFABS ============*/

void ComponentHandler::ModifyComponentOfEntityToPrefab(Entity& entity, Cookie::Resources::ResourcesManager& resourcesManager, std::string& namePrefab)
{
	if (entity.namePrefab == namePrefab)
		return;

	Resources::Prefab* prefab = resourcesManager.prefabs[namePrefab].get();

	int idEntity = entity.id;

	if (!(idEntity & C_SIGNATURE::TRANSFORM))
		AddComponent(entity, C_SIGNATURE::TRANSFORM);

	componentTransforms[idEntity].rot = prefab->rotation;
	componentTransforms[idEntity].scale = prefab->scale;

	if (!(idEntity & C_SIGNATURE::MODEL))
		AddComponent(entity, C_SIGNATURE::MODEL);

	componentModels[idEntity].mesh = resourcesManager.meshes[prefab->nameMesh].get();
	componentModels[idEntity].albedo = resourcesManager.textures[prefab->nameTexture].get();
	//componentModels[idEntity].shader = resourcesManager.shaders[prefab->nameShader]; 

	//if (!(idEntity & C_SIGNATURE::SCRIPT))
	//	AddComponentScript(entity);

	//componentScripts[idEntity] = Cookie::Resources::Script();

	entity.namePrefab = namePrefab;
}