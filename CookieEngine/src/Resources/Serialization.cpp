#include <iostream>
#include <fstream>
#include <iomanip>
#include "Physics/PhysicsHandle.hpp"
#include "Coordinator.hpp"
#include "Resources/Scene.hpp"
#include "Game.hpp"
#include "Resources/Serialization.hpp"
#include "Resources/Prefab.hpp"
#include <bitset>
#include <filesystem>

using namespace Cookie;
using namespace Cookie::Resources;
using namespace Cookie::Resources::Serialization;

void Cookie::Resources::Serialization::Save::ToJson(json& js, const Cookie::ECS::EntityHandler& entity)
{
	for (int i = 0; i < entity.livingEntities; i++)
		js["EntityHandler"] += json{ { "entity", { { "id", entity.entities[i].id }, { "signature", entity.entities[i].signature }, { "name", entity.entities[i].name }, { "namePrefab", entity.entities[i].namePrefab } } } };
}

void Cookie::Resources::Serialization::Save::ToJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler& component, Cookie::Resources::ResourcesManager& resourcesManager)
{
	for (int i = 0; i < entity.livingEntities; i++)
	{
		if (entity.entities[i].signature & SIGNATURE_TRANSFORM)
		{
			Cookie::ECS::ComponentTransform transform = component.GetComponentTransform(entity.entities[i].id);

			if (entity.entities[i].namePrefab != "NONE")
			{
				js["ComponentHandler"]["Transform"] += json{ { "localTRS", { { "translate", transform.pos.e } } } };
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->rotation != transform.rot)
					js["ComponentHandler"]["Transform"].at(js["ComponentHandler"]["Transform"].size() - 1).at("localTRS")["rotation"] = transform.rot.e;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->scale != transform.scale)
					js["ComponentHandler"]["Transform"].at(js["ComponentHandler"]["Transform"].size() - 1).at("localTRS")["scale"] = transform.scale.e;
			}
			else
			{
				js["ComponentHandler"]["Transform"] += json{ { "localTRS", { { "translate", transform.pos.e },
													{ "rotation", transform.rot.e },
													{ "scale", transform.scale.e } } } };
			}
		}
		if (entity.entities[i].signature & SIGNATURE_MODEL)
		{
			Cookie::ECS::ComponentModel model = component.GetComponentModel(entity.entities[i].id);
			if (entity.entities[i].namePrefab != "NONE")
			{
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->nameMesh != model.mesh.get()->name)
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size()]["model"] = model.mesh.get()->name;
				else
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size()]["model"] = 0;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->nameTexture != model.texture.get()->name)
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size() - 1]["texture"] = model.texture.get()->name;
				else
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size() - 1]["texture"] = 0;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->nameShader != model.shader.get()->name)
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size() - 1]["shader"] = model.shader.get()->name;
				else
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size() - 1]["shader"] = 0;
			}
			else
			{
				js["ComponentHandler"]["Model"] += json{ { "model", model.mesh != nullptr ? model.mesh.get()->name : "NO MESH" },
												{ "shader", model.shader.get()->name },
												{ "texture", model.texture != nullptr ? model.texture.get()->name : "NO TEXTURE" } };
			}
		}
	}
}


void Cookie::Resources::Serialization::Save::SaveScene(Cookie::Resources::Scene& actScene, Cookie::Resources::ResourcesManager& resourcesManager)
{
	std::ofstream file(actScene.filepath);

	json js;

	//Name Type
	{
		js["Name"] = actScene.name;
		js["Type"] = "map";
	}

	//Size tiles
	{
		js["Tiles"] = json{ { "widthTile", actScene.tiles.widthTile }, { "depthTile", actScene.tiles.depthTile },
							{ "widthTileProp", actScene.tiles.widthTileProp }, { "depthTileProp", actScene.tiles.depthTileProp } };
	}


	if (actScene.entityHandler.livingEntities > 0)
	{
		//Entities
		{
			Cookie::Resources::Serialization::Save::ToJson(js, actScene.entityHandler);
		}

		//Components
		{
			Cookie::Resources::Serialization::Save::ToJson(js, actScene.entityHandler, actScene.componentHandler, resourcesManager);
		}
	}

	file << std::setw(4) << js << std::endl;
}

 void Cookie::Resources::Serialization::Save::SavePrefab(const std::shared_ptr<Prefab>& prefab)
 {
	 std::ofstream file(prefab->filepath);

	 //std::cout << prefab->filepath << "\n";

	 json js;

	 js["Mesh"] = prefab->nameMesh;
	 js["Signature"] = prefab->signature;
	 js["Name"] = prefab->name;
	 js["Rotation"] = prefab->rotation.e;
	 js["Scale"] = prefab->scale.e;
	 js["Script"] = prefab->nameScript;
	 js["Shader"] = prefab->nameShader;
	 js["Texture"] = prefab->nameTexture;

	 file << std::setw(4) << js << std::endl;
 }

 void Cookie::Resources::Serialization::Save::SaveAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 for (std::unordered_map<std::string, std::shared_ptr<Prefab>>::iterator prefab = resourcesManager.prefabs.begin(); prefab != resourcesManager.prefabs.end(); prefab++)
		 Resources::Serialization::Save::SavePrefab(prefab->second);
 }


 //------------------------------------------------------------------------------------------------------------------

 void Cookie::Resources::Serialization::Load::FromJson(json& js, Cookie::ECS::EntityHandler& entity)
 {
	 for (int i = 0; i < entity.livingEntities; i++)
	 {
		 json newEntity = js["EntityHandler"][i].at("entity");
		 entity.entities[i] = (Cookie::ECS::Entity(newEntity.at("id").get<int>(), newEntity.at("signature").get<int>(), 
								newEntity.at("name").get<std::string>(), newEntity.at("namePrefab").get<std::string>()));
		 entity.entities[i].namePrefab = newEntity.at("namePrefab").get<std::string>();
	 }
 }

 void Cookie::Resources::Serialization::Load::FromJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler& component, Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 for (int i = 0; i < entity.livingEntities; i++)
	 {
		 if (entity.entities[i].signature & SIGNATURE_TRANSFORM)
		 {
			 Cookie::ECS::ComponentTransform transform;
			 json TRS = js["ComponentHandler"]["Transform"][i].at("localTRS");
			 TRS.at("translate").get_to(transform.pos.e);
			 
			 if (TRS.contains("rotation"))
				 TRS.at("rotation").get_to(transform.rot.e);
			 else if (entity.entities[i].namePrefab != "NONE")
				 transform.rot = resourcesManager.prefabs[entity.entities[i].namePrefab].get()->rotation;
			 
			 if (TRS.contains("scale"))
				TRS.at("scale").get_to(transform.scale.e);
			 else if (entity.entities[i].namePrefab != "NONE")
				 transform.scale = resourcesManager.prefabs[entity.entities[i].namePrefab].get()->scale;
			 
			 component.componentTransforms[entity.entities[i].id] = transform;
		 }
		 if (entity.entities[i].signature & SIGNATURE_MODEL)
		 {
			 std::string test;
			 json model = js["ComponentHandler"]["Model"][i];
			 if (model.at("model").is_string())
				component.componentModels[entity.entities[i].id].mesh = resourcesManager.meshes[(model.at("model").get<std::string>())];
			 else if (entity.entities[i].namePrefab != "NONE")
				 component.componentModels[entity.entities[i].id].mesh =
						resourcesManager.meshes[resourcesManager.prefabs[entity.entities[i].namePrefab].get()->nameMesh];

			 if (model.at("texture").is_string())
				component.componentModels[entity.entities[i].id].texture = resourcesManager.textures[(model.at("texture").get<std::string>())];
			 else if (entity.entities[i].namePrefab != "NONE")
				 component.componentModels[entity.entities[i].id].texture = 
									resourcesManager.textures[resourcesManager.prefabs[entity.entities[i].namePrefab].get()->nameTexture];

			 if (model.at("shader").is_string())
				component.componentModels[entity.entities[i].id].shader = resourcesManager.shaders[model.at("shader").get<std::string>()];
			 else if (entity.entities[i].namePrefab != "NONE")
				 component.componentModels[entity.entities[i].id].shader =
									resourcesManager.shaders[resourcesManager.prefabs[entity.entities[i].namePrefab].get()->nameShader];
		 }
		 if (entity.entities[i].signature & SIGNATURE_PHYSICS)
		 {
			 component.componentPhysics[entity.entities[i].id].Set(component.componentTransforms[entity.entities[i].id]);
			 component.componentPhysics[entity.entities[i].id].physBody = Physics::PhysicsHandle::physSim->createRigidBody(component.componentPhysics[entity.entities[i].id].physTransform);
		 }
	 }
 }

 std::shared_ptr<Scene> Cookie::Resources::Serialization::Load::LoadScene(const char* filepath, Game& game)
 {
	 std::shared_ptr<Resources::Scene> newScene = std::make_shared<Resources::Scene>();
	 Physics::PhysicsHandle::physSim = newScene->physSim.worldSim;
	 std::ifstream file(filepath);

	 if (!file.is_open())
	 {
		 std::cout << "DON'T FIND THE FILE\n";
		 return nullptr;
	 }
	 
	 //std::cout << filepath << "\n";

	 json js;
	 file >> js;
	 
	 if (js.contains("Name"))
	 {
		 //name
		 {
			  js["Name"].get_to(newScene->name);
		 }
	 }
	 
	 
	 if (js.contains("Tiles"))
	 {
		 //tiles
		 {
			 //----------------------CHECK--------------------------------------
			 js["Tiles"].at("widthTile").get_to(newScene->tiles.widthTile);
			 js["Tiles"].at("depthTile").get_to(newScene->tiles.depthTile);
			 js["Tiles"].at("widthTileProp").get_to(newScene->tiles.widthTileProp);
			 js["Tiles"].at("depthTileProp").get_to(newScene->tiles.depthTileProp);
			 newScene->ChangeNumberOfTiles(newScene->tiles.widthTile, newScene->tiles.depthTile);
			 newScene->tiles.ReInitAllTiles();
		 }
	 } 
	 
	 if (js.contains("EntityHandler"))
	 {
		 int newSizeEntities = js["EntityHandler"].size();
		 for (int i = newSizeEntities; i < newSizeEntities + newScene->entityHandler.livingEntities - newSizeEntities; i++)
		 {
			 if (newScene->entityHandler.entities[i].signature & SIGNATURE_TRANSFORM)
				 newScene->componentHandler.componentTransforms[newScene->entityHandler.entities[i].id].ToDefault();
			 if (newScene->entityHandler.entities[i].signature & SIGNATURE_MODEL)
				 newScene->componentHandler.componentModels[newScene->entityHandler.entities[i].id].ToDefault();
			 newScene->entityHandler.entities[i] = Cookie::ECS::Entity(i);
		 }

		 //entities
		 {
			 newScene->entityHandler.livingEntities = newSizeEntities;
			 Cookie::Resources::Serialization::Load::FromJson(js, newScene->entityHandler);
		 }

		 //component
		 {
			 Cookie::Resources::Serialization::Load::FromJson(js, newScene->entityHandler, newScene->componentHandler, game.resources);
		 }
	 }
		 
	 newScene->filepath = filepath;

	 return newScene;
 }

 void Cookie::Resources::Serialization::Load::LoadAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 std::vector<std::string> filesPath;
	 for (const fs::directory_entry& path : fs::directory_iterator("Assets/Prefabs"))
	 {
		 if (path.path().string().find(".PAsset") != std::string::npos)
			filesPath.push_back(path.path().string());
	 }

	 for (int i = 0; i < filesPath.size(); i++)
	 {
		 std::cout << filesPath[i] << "\n";

		 std::ifstream file(filesPath[i]);

		 if (!file.is_open())
		 {
			 std::cout << "DON'T FIND THE FILE\n";
			 continue;
		 }

		 json js;
		 file >> js;

		 Cookie::Resources::Prefab newPrefab;

		 if (js.contains("Name"))
			 newPrefab.name = js["Name"];

		 if (js.contains("Mesh"))
			 newPrefab.nameMesh = js["Mesh"];

		 if (js.contains("Texture"))
			 newPrefab.nameTexture = js["Texture"];

		 if (js.contains("Shader"))
			 newPrefab.nameShader = js["Shader"];

		 if (js.contains("Rotation"))
			 js["Rotation"].get_to(newPrefab.rotation.e);

		 if (js.contains("Scale"))
			 js["Scale"].get_to(newPrefab.scale.e);

		 if (js.contains("Script"))
			 newPrefab.nameScript = js["Script"];

		 if (js.contains("Signature"))
			 newPrefab.signature = js["Signature"];

		 newPrefab.filepath = filesPath[i];

		 resourcesManager.prefabs[newPrefab.name] = std::make_shared<Prefab>(newPrefab);
	 }
 }
