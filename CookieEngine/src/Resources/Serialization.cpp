#include <iostream>
#include <fstream>
#include <iomanip>
#include <reactphysics3d/reactphysics3d.h>
#include "Coordinator.hpp"
#include "Resources/Scene.hpp"
#include "Game.hpp"
#include "Resources/Serialization.hpp"
#include <bitset>
#include <filesystem>

using namespace Cookie;
using namespace Cookie::Resources;
using namespace Cookie::Resources::Serialization;

 void Cookie::Resources::Serialization::Save::ToJson(json& js, const Cookie::ECS::EntityHandler& entity)
{
	for (int i = 0; i < entity.livingEntities; i++)
		js["EntityHandler"] += json{ { "entity", { { "id", entity.entities[i].id }, { "signature", entity.entities[i].signature }, { "name", entity.entities[i].name } } } };
}

 void Cookie::Resources::Serialization::Save::ToJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler component)
 {
	 for (int i = 0; i < entity.livingEntities; i++)
	 {
		 if (entity.entities[i].signature & SIGNATURE_TRANSFORM)
		 {
			 Cookie::ECS::ComponentTransform transform = component.GetComponentTransform(entity.entities[i].id);
			 js["ComponentHandler"]["Transform"] += json{ { "localTRS", { { "translate", transform.localTRS.pos.e }, 
														{ "rotation", transform.localTRS.rot.e }, 
														{ "scale", transform.localTRS.scale.e } } } };
		 }
		 if (entity.entities[i].signature & SIGNATURE_MODEL)
		 {
			 Cookie::ECS::ComponentModel model = component.GetComponentModel(entity.entities[i].id);
			 js["ComponentHandler"]["Model"] += json{ { "model", model.mesh != nullptr ? model.mesh.get()->name: "NO MESH" }, 
													{ "shader", "default" },
													{ "texture", model.texture != nullptr ? model.texture.get()->name: "NO TEXTURE" } };
		 }
	 }
 }


 void Cookie::Resources::Serialization::Save::SaveScene(const Cookie::Resources::Scene& actScene)
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
			Cookie::Resources::Serialization::Save::ToJson(js, actScene.entityHandler, actScene.componentHandler);
		}
	}

	file << std::setw(4) << js << std::endl;
}

 //------------------------------------------------------------------------------------------------------------------

 void Cookie::Resources::Serialization::Load::FromJson(json& js, Cookie::ECS::EntityHandler& entity)
 {
	 for (int i = 0; i < entity.livingEntities; i++)
		 entity.entities[i] = (Cookie::ECS::Entity(js["EntityHandler"][i].at("entity").at("id").get<int>(), js["EntityHandler"][i].at("entity").at("signature").get<int>(), js["EntityHandler"][i].at("entity").at("name").get<std::string>()));
 }

 void Cookie::Resources::Serialization::Load::FromJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler& component, Cookie::Resources::ResourcesManager resourcesManager)
 {
	 for (int i = 0; i < entity.livingEntities; i++)
	 {
		 if (entity.entities[i].signature & SIGNATURE_TRANSFORM)
		 {
			 Cookie::ECS::ComponentTransform transform;
			 js["ComponentHandler"]["Transform"][i].at("localTRS").at("translate").get_to(transform.localTRS.pos.e);
			 js["ComponentHandler"]["Transform"][i].at("localTRS").at("rotation").get_to(transform.localTRS.rot.e);
			 js["ComponentHandler"]["Transform"][i].at("localTRS").at("scale").get_to(transform.localTRS.scale.e);
			 component.componentTransforms[entity.entities[i].id].localTRS = transform.localTRS;
		 }
		 if (entity.entities[i].signature & SIGNATURE_MODEL)
		 {
			 std::string test;
			 component.componentModels[entity.entities[i].id].mesh = resourcesManager.meshes[(js["ComponentHandler"]["Model"][i].at("model").get<std::string>())];
			 component.componentModels[entity.entities[i].id].texture = resourcesManager.textures[(js["ComponentHandler"]["Model"][i].at("texture").get<std::string>())];
			 component.componentModels[entity.entities[i].id].shader = resourcesManager.shaders["dfltShader"];//resourcesManager.GetMesh(js["ComponentHandler"]["Model"][i].at("shader").get<std::string>());
		 }
	 }
 }

 std::shared_ptr<Scene> Cookie::Resources::Serialization::Load::LoadScene(const char* filepath, Game& game)
 {
	 std::shared_ptr<Resources::Scene> newScene = std::make_shared<Resources::Scene>(game.physCom);
	 std::ifstream file(filepath);

	 if (!file.is_open())
	 {
		 std::cout << "DON'T FIND THE FILE\n";
		 return nullptr;
	 }
	 
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
			 /*if (newScene->entityHandler.entities[i].signature & SIGNATURE_RIGIDBODY)
				 newScene->componentHandler.componentRigidBodies[newScene->entityHandler.entities[i].id].ToDefault();*/
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

		 game.resources.scenes[newScene->name] = (newScene);
	 }
		 
	 newScene->filepath = filepath;

	 return newScene;
 }

 void Cookie::Editor::Serialization::Load::LoadAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 std::vector<std::string> filesPath;
	 for (const fs::directory_entry& path : fs::directory_iterator("Assets/Prefabs"))
	 {
		 if (path.path().string().find(".PAsset") != std::string::npos)
			filesPath.push_back(path.path().string());
	 }
	 
	 for (int i = 0; i < filesPath.size(); i++)
	 {
		 std::cout << filesPath[i] << "\t";

		 std::ifstream file(filesPath[i]);

		 if (!file.is_open())
		 {
			 std::cout << "DON'T FIND THE FILE\n";
			 return;
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
			 js["Scale"].get_to(newPrefab.rotation.e);

		 if (js.contains("Script"))
			 newPrefab.nameMesh = js["Mesh"];
	 }

		
	 //std::size_t found = 
	 //resourcesManager.prefabs
 }
