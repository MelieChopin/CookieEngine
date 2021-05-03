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

/*============ PREFABS ============*/

void ComponentHandler::ModifyComponentOfEntityToPrefab(Entity& entity, Cookie::Resources::ResourcesManager& resourcesManager, std::string& namePrefab)
{
	if (entity.namePrefab == namePrefab)
		return;

	Resources::Prefab* prefab = resourcesManager.prefabs[namePrefab].get();

	int idEntity = entity.id;
	
	if (!(idEntity & SIGNATURE_TRANSFORM))
		AddComponentTransform(entity);

	componentTransforms[idEntity].rot = prefab->rotation;
	componentTransforms[idEntity].scale = prefab->scale;

	if (!(idEntity & SIGNATURE_MODEL))
		AddComponentModel(entity);

	componentModels[idEntity].mesh = resourcesManager.meshes[prefab->nameMesh].get();
	componentModels[idEntity].albedo = resourcesManager.textures[prefab->nameTexture].get();
	//componentModels[idEntity].shader = resourcesManager.shaders[prefab->nameShader]; 

	//if (!(idEntity & SIGNATURE_SCRIPT))
	//	AddComponentScript(entity);

	//componentScripts[idEntity] = Cookie::Resources::Script();

	entity.namePrefab = namePrefab;
}