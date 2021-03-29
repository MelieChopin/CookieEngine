#include "ECS/ComponentHandler.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Resources/Prefab.hpp"

using namespace Cookie::ECS;

void ComponentHandler::ModifyComponentOfEntityToPrefab(Entity& entity, Cookie::Resources::ResourcesManager& resourcesManager, std::string& namePrefab)
{
	if (entity.namePrefab == namePrefab)
		return;

	Resources::Prefab* prefab = resourcesManager.prefabs[namePrefab].get();

	int idEntity = entity.id;
	
	if (!(idEntity & SIGNATURE_TRANSFORM))
		AddComponentTransform(entity);

	componentTransforms[idEntity].localTRS.rot = prefab->rotation;
	componentTransforms[idEntity].localTRS.scale = prefab->scale;

	if (!(idEntity & SIGNATURE_MODEL))
		AddComponentModel(entity);

	componentModels[idEntity].mesh = resourcesManager.meshes[prefab->nameMesh];
	componentModels[idEntity].texture = resourcesManager.textures[prefab->nameTexture];
	//componentModels[idEntity].shader = resourcesManager.shaders[prefab->nameShader]; 

	//if (!(idEntity & SIGNATURE_SCRIPT))
	//	AddComponentScript(entity);

	//componentScripts[idEntity] = Cookie::Resources::Script();

	entity.namePrefab = namePrefab;
}