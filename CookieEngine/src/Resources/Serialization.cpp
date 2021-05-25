
#include "Physics/PhysicsHandle.hpp"
#include "Game.hpp"

#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Scene.hpp"

#include "SoundManager.hpp"
#include "Resources/ResourcesManager.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Resources/Serialization.hpp"
#include "Resources/Prefab.hpp"
#include <reactphysics3d/reactphysics3d.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <filesystem>

namespace fs = std::filesystem;
using namespace Cookie::Resources;
using namespace Cookie::ECS;
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
		if (entity.entities[i].signature & C_SIGNATURE::TRANSFORM)
		{
			Cookie::ECS::ComponentTransform transform = component.GetComponentTransform(entity.entities[i].id);
			json& trans = js["ComponentHandler"]["Transform"];
			if (entity.entities[i].namePrefab != "NONE")
			{
				trans += json{ { "localTRS", { { "translate", transform.pos.e } } } };
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.rot != transform.rot)
					trans.at(trans.size() - 1).at("localTRS")["rotation"] = transform.rot.e;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.scale != transform.scale)
					trans.at(trans.size() - 1).at("localTRS")["scale"] = transform.scale.e;
			}
			else
			{
				trans += json{ { "localTRS", { { "translate", transform.pos.e },
													{ "rotation", transform.rot.e },
													{ "scale", transform.scale.e } } } };
			}
		}
		if (entity.entities[i].signature & C_SIGNATURE::MODEL)
		{
			Cookie::ECS::ComponentModel model = component.GetComponentModel(entity.entities[i].id);
			json& modelJ = js["ComponentHandler"]["Model"];
			if (entity.entities[i].namePrefab != "NONE")
			{
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.mesh->name != model.mesh->name)
					modelJ[modelJ.size()]["model"] = model.mesh->name;
				else
					modelJ[modelJ.size()]["model"] = 0;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.albedo->name != model.albedo->name)
					modelJ[modelJ.size() - 1]["texture"] = model.albedo->name;
				else
					modelJ[modelJ.size() - 1]["texture"] = 0;
			}
			else
			{
				modelJ += json{ { "model", model.mesh != nullptr ? model.mesh->name : "NO MESH" },
												{ "texture", model.albedo != nullptr ? model.albedo->name : "NO TEXTURE" } };
			}
		}
		if (entity.entities[i].signature & C_SIGNATURE::PHYSICS)
		{
			int index = js["PhysicHandler"].size();
			SavePhysic(js["PhysicHandler"][index], component.GetComponentPhysics(entity.entities[i].id));
		}
		if (entity.entities[i].signature & C_SIGNATURE::GAMEPLAY)
		{
			ComponentGameplay gameplay = component.GetComponentGameplay(entity.entities[i].id);
			int index = js["Gameplay"].size();
			json& game = js["Gameplay"][index];
			game["TeamName"] = gameplay.teamName;
			game["SignatureGameplay"] = gameplay.signatureGameplay;
			game["Type"] = gameplay.type;
			game["Cost"]["CostPrimary"] = gameplay.cost.costPrimary;
			game["Cost"]["CostSecondary"] = gameplay.cost.costSecondary;
			game["Cost"]["CostSupply"] = gameplay.cost.costSupply;
			game["Cost"]["TimeToProduce"] = gameplay.cost.timeToProduce;

			game["CGPLive"]["Life"] = gameplay.componentLive.life;
			game["CGPLive"]["Armor"] = gameplay.componentLive.armor;

			game["CGPAttack"]["NeedToAttack"] = gameplay.componentAttack.needToAttack;
			game["CGPAttack"]["AttackDamage"] = gameplay.componentAttack.attackDamage;
			game["CGPAttack"]["AttackSpeed"] = gameplay.componentAttack.attackSpeed;
			game["CGPAttack"]["AttackRange"] = gameplay.componentAttack.attackRange;

			game["CGPMove"]["MoveSpeed"] = gameplay.componentMove.moveSpeed;
			game["CGPMove"]["isFlying"] = gameplay.componentMove.isFlying;

			for (int i = 0; i < gameplay.componentProducer.possibleUnits.size(); i++)
				game["CGPProducer"]["name"] += gameplay.componentProducer.possibleUnits[i]->name;

			game["CGPProducer"]["TileSize"] = gameplay.componentProducer.tileSize.e;

			for (int i = 0; i < gameplay.componentWorker.possibleBuildings.size(); i++)
				game["CGPWorker"]["name"] += gameplay.componentWorker.possibleBuildings[i]->name;
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
	/*
	//Map
	{
		json& map = js["Map"];
		Cookie::Resources::Map actMap = actScene.map;
		map["tilesNb"] = actMap.tilesNb.e;
		map["tilesSize"] = actMap.tilesSize.e;
		map["tilesNbIcons"] = actMap.tilesNbIcons.e;

		map["trs"]["pos"] = actMap.trs.pos.e;
		map["trs"]["rot"] = actMap.trs.rot.e;
		map["trs"]["scale"] = actMap.trs.scale.e;

		map["model"]["mesh"] = actMap.model.mesh.get()->name;
		map["model"]["texture"] = actMap.model.texture.get()->name;
		map["model"]["shader"] = actMap.model.shader.get()->name;

		//SavePhysic(map, actMap.physic);
	}*/


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

void Cookie::Resources::Serialization::Save::SavePrefab(const Prefab* const & prefab)
 {
	 std::ofstream file(prefab->filepath);

	 json js;

	 js["Signature"] = prefab->signature;
	 js["Name"] = prefab->name;
	 
	 if (prefab->signature & C_SIGNATURE::TRANSFORM)
	 {
		 //Transform
		 json& trans = js["Transform"];
		 trans["Rotation"] = prefab->transform.rot.e;
		 trans["Scale"] = prefab->transform.scale.e;
	 }
	 
	 if (prefab->signature & C_SIGNATURE::MODEL)
	 {
		 //Model
		 json& model = js["Model"];
		 if (prefab->model.mesh != nullptr)
			 model["Mesh"] = prefab->model.mesh->name;
		 else
			 model["Mesh"] = 0;
		 if (prefab->model.albedo != nullptr)
			 model["Texture"] = prefab->model.albedo->name;
		 else
			 model["Texture"] = 0;
	 }

	 if (prefab->signature & C_SIGNATURE::PHYSICS)
	 {
		 if (prefab->physics.physBody != nullptr)
		 {
			 json& rigid = js["Rigidbody"];
			 rigid["type"] = prefab->physics.physBody->getType();
			 rigid["angularDamping"] = prefab->physics.physBody->getAngularDamping();
			 rigid["linearDamping"] = prefab->physics.physBody->getLinearDamping();
			 rigid["mass"] = prefab->physics.physBody->getMass();
			 rigid["active"] = prefab->physics.physBody->isActive();
			 rigid["allowedToSleep"] = prefab->physics.physBody->isAllowedToSleep();
			 rigid["sleeping"] = prefab->physics.physBody->isSleeping();
			 rigid["gravityEnabled"] = prefab->physics.physBody->isGravityEnabled();
		 }
	 }
	 else
		 js["Rigidbody"] = "nullptr";

	 if (prefab->signature & C_SIGNATURE::PHYSICS)
	 {
		 if (prefab->physics.physColliders.size() != 0)
		 {
			 for (int i = 0; i < prefab->physics.physColliders.size(); i++)
			 {
				 ::reactphysics3d::Collider* actCollider = prefab->physics.physColliders[i];
				 json& collider = js["Colliders"][i];
				 if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::SPHERE)
				 {
					 float radius = static_cast<::reactphysics3d::SphereShape*>(actCollider->getCollisionShape())->getRadius();
					 collider["type"] = "Sphere";
					 collider["radius"] = radius;
				 }
				 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::CAPSULE)
				 {
					 ::reactphysics3d::CapsuleShape* capsule = static_cast<::reactphysics3d::CapsuleShape*>(actCollider->getCollisionShape());
					 collider["type"] = "Capsule";
					 collider["radius"] = capsule->getRadius();
					 collider["height"] = capsule->getHeight();
				 }
				 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::BOX)
				 {
					 ::reactphysics3d::Vector3 box = static_cast<::reactphysics3d::BoxShape*>(actCollider->getCollisionShape())->getHalfExtents();
					 collider["type"] = "Box";
					 collider["HalfExtents"] = { box.x, box.y, box.z };
				 }

				 ::reactphysics3d::Material& mat = actCollider->getMaterial();

				 ::reactphysics3d::Vector3		vec = actCollider->getLocalToBodyTransform().getPosition();
				 ::reactphysics3d::Quaternion	quat = actCollider->getLocalToBodyTransform().getOrientation();
				 collider["transform"]["pos"] = { vec.x, vec.y, vec.z };
				 collider["transform"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
				 collider["bounciness"] = mat.getBounciness();
				 collider["frictionCoeff"] = mat.getFrictionCoefficient();
				 collider["density"] = mat.getMassDensity();
				 collider["rollingResistance"] = mat.getRollingResistance();
			 }
		 }
	 }
	 else
		 js["Colliders"] = "nullptr";

	 if (prefab->signature & C_SIGNATURE::GAMEPLAY)
	 {
		 ComponentGameplay gameplay = prefab->gameplay;
		 json& gp = js["Gameplay"];
		 gp["TeamName"] = gameplay.teamName;
		 gp["Type"] = gameplay.type;
		 gp["SignatureGameplay"] = gameplay.signatureGameplay;

		 json& cost = js["Gameplay"]["Cost"];
		 cost["CostPrimary"] = gameplay.cost.costPrimary;
		 cost["CostSecondary"] = gameplay.cost.costSecondary;
		 cost["CostSupply"] = gameplay.cost.costSupply;
		 cost["TimeToProduce"] = gameplay.cost.timeToProduce;

		 json& live = js["Gameplay"]["CGPLive"];
		 live["Life"] = gameplay.componentLive.life;
		 live["Armor"] = gameplay.componentLive.armor;

		 json& attack = js["Gameplay"]["CGPAttack"];
		 attack["NeedToAttack"] = gameplay.componentAttack.needToAttack;
		 attack["AttackDamage"] = gameplay.componentAttack.attackDamage;
		 attack["AttackSpeed"] = gameplay.componentAttack.attackSpeed;
		 attack["AttackCooldown"] = gameplay.componentAttack.attackCooldown;
		 attack["AttackRange"] = gameplay.componentAttack.attackRange;

		 json& move = js["Gameplay"]["CGPMove"];
		 move["MoveSpeed"] = gameplay.componentMove.moveSpeed;
		 move["isFlying"] = gameplay.componentMove.isFlying;

		 json& produ = js["Gameplay"]["CGPProducer"];
		 for (int i = 0; i < gameplay.componentProducer.possibleUnits.size(); i++)
			 produ["name"] += gameplay.componentProducer.possibleUnits[i]->name;

		 produ["TileSize"] = gameplay.componentProducer.tileSize.e;

		 json& worker = js["Gameplay"]["CGPWorker"];
		 for (int i = 0; i < gameplay.componentWorker.possibleBuildings.size(); i++)
			 worker["name"] += gameplay.componentWorker.possibleBuildings[i]->name;
	 }

	 file << std::setw(4) << js << std::endl;
 }

void Cookie::Resources::Serialization::Save::SaveAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 for (std::unordered_map<std::string, std::unique_ptr<Prefab>>::iterator prefab = resourcesManager.prefabs.begin(); prefab != resourcesManager.prefabs.end(); prefab++)
		 Resources::Serialization::Save::SavePrefab(prefab->second.get());
 }

void Cookie::Resources::Serialization::Save::SaveTexture(std::string& name, Cookie::Core::Math::Vec4& color)
 {
	 std::ofstream file("Assets/Textures/" + name + ".TAsset");

	 json js;

	 js["color"] = color.e;
	 js["name"] = name;

	 file << std::setw(4) << js << std::endl;
 }

void Cookie::Resources::Serialization::Save::SavePhysic(json& js, Cookie::ECS::ComponentPhysics& physics)
 {
	 std::vector<::reactphysics3d::Collider*> colliders = physics.physColliders;
	 for (int j = 0; j < colliders.size(); j++)
	 {
		 ::reactphysics3d::Collider* actCollider = colliders[j];
		 json& colliders = js["Colliders"][j];
		 if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::SPHERE)
		 {
			 float radius = static_cast<::reactphysics3d::SphereShape*>(actCollider->getCollisionShape())->getRadius();
			 colliders["type"] = "Sphere";
			 colliders["radius"] = radius;
		 }
		 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::CAPSULE)
		 {
			 ::reactphysics3d::CapsuleShape* capsule = static_cast<::reactphysics3d::CapsuleShape*>(actCollider->getCollisionShape());
			 colliders["type"] = "Capsule";
			 colliders["radius"] = capsule->getRadius();
			 colliders["height"] = capsule->getHeight();
		 }
		 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::BOX)
		 {
			 ::reactphysics3d::Vector3 box = static_cast<::reactphysics3d::BoxShape*>(actCollider->getCollisionShape())->getHalfExtents();
			 colliders["type"] = "Box";
			 colliders["HalfExtents"] = { box.x, box.y, box.z };
		 }

		 ::reactphysics3d::Material& mat = actCollider->getMaterial();

		 ::reactphysics3d::Vector3		vec = actCollider->getLocalToBodyTransform().getPosition();
		 ::reactphysics3d::Quaternion	quat = actCollider->getLocalToBodyTransform().getOrientation();
		 colliders["transform"]["pos"] = { vec.x, vec.y, vec.z };
		 colliders["transform"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
		 colliders["bounciness"] = mat.getBounciness();
		 colliders["frictionCoeff"] = mat.getFrictionCoefficient();
		 colliders["density"] = mat.getMassDensity();
		 colliders["rollingResistance"] = mat.getRollingResistance();
	 }

	 ::reactphysics3d::RigidBody*& rigibody = physics.physBody;

	 json& rigidbody = js["Rigidbody"];
	 rigidbody["type"] = rigibody->getType();
	 rigidbody["angularDamping"] = rigibody->getAngularDamping();
	 rigidbody["linearDamping"] = rigibody->getLinearDamping();
	 rigidbody["mass"] = rigibody->getMass();
	 rigidbody["active"] = rigibody->isActive();
	 rigidbody["allowedToSleep"] = rigibody->isAllowedToSleep();
	 rigidbody["sleeping"] = rigibody->isSleeping();
	 rigidbody["gravityEnabled"] = rigibody->isGravityEnabled();

	 ::reactphysics3d::Transform physTrans = physics.physTransform;
	 ::reactphysics3d::Vector3 pos = physTrans.getPosition();
	 ::reactphysics3d::Quaternion quat = physTrans.getOrientation();

	 js["physicTRS"]["position"] = { pos.x, pos.y, pos.z };
	 js["physicTRS"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
 }

void Cookie::Resources::Serialization::Save::SaveVolumAndModeMusic(std::string key)
{
	Cookie::Resources::Sound sound = *(*(Cookie::Resources::SoundManager::sounds))[key].get();

	std::string filepath = sound.filepath;

	std::size_t pos = filepath.find(".mp3");
	std::size_t end = filepath.length();

	filepath.replace(pos, end, ".MAsset");

	std::ofstream file(filepath);

	json js;

	std::cout << sound.vol;
	js["Volume"] = sound.vol;
	js["Mode"] = sound.mode;
	js["Pos"] = sound.pos.e;

	file << std::setw(4) << js << std::endl;
}

void Cookie::Resources::Serialization::Save::SaveVolumAndModeMusic(Sound* const & sound)
{
	std::string filepath = sound->filepath;

	std::size_t pos = filepath.find(".mp3");
	std::size_t end = filepath.length();

	filepath.replace(pos, end, ".MAsset");

	std::ofstream file(filepath);

	json js;

	std::cout << sound->vol;
	js["Volume"] = sound->vol;
	js["Mode"] = sound->mode;
	js["Pos"] = sound->pos.e;

	file << std::setw(4) << js << std::endl;
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

void Cookie::Resources::Serialization::Load::FromJson(json& js, const Cookie::ECS::EntityHandler& entity,
	 Cookie::ECS::ComponentHandler& component, Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 int indexOfPhysic = 0;
	 int indexOfGameplay = 0;
	 for (int i = 0; i < entity.livingEntities; i++)
	 {
		 if (entity.entities[i].signature & C_SIGNATURE::TRANSFORM)
		 {
			 Cookie::ECS::ComponentTransform transform;
			 json TRS = js["ComponentHandler"]["Transform"][i].at("localTRS");
			 TRS.at("translate").get_to(transform.pos.e);
			 
			 if (TRS.contains("rotation"))
				 TRS.at("rotation").get_to(transform.rot.e);
			 else if (entity.entities[i].namePrefab != "NONE")
				 transform.rot = resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.rot;
			 
			 if (TRS.contains("scale"))
				TRS.at("scale").get_to(transform.scale.e);
			 else if (entity.entities[i].namePrefab != "NONE")
			 {
				 
				 transform.scale = resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.scale;
				  transform.scale.Debug();
			 }
			 
			 transform.trsHasChanged = true;
			 component.GetComponentTransform(entity.entities[i].id) = transform;
		 }
		 if (entity.entities[i].signature & C_SIGNATURE::MODEL)
		 {
			 json model = js["ComponentHandler"]["Model"][i];
			 if (model.at("model").is_string())
				component.GetComponentModel(entity.entities[i].id).mesh = resourcesManager.meshes[(model.at("model").get<std::string>())].get();
			 else if (entity.entities[i].namePrefab != "NONE")
				 component.GetComponentModel(entity.entities[i].id).mesh =
						resourcesManager.meshes[resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.mesh->name].get();

			 if (model.at("texture").is_string())
				component.GetComponentModel(entity.entities[i].id).albedo = resourcesManager.textures2D[(model.at("texture").get<std::string>())].get();
			 else if (entity.entities[i].namePrefab != "NONE")
				 component.GetComponentModel(entity.entities[i].id).albedo =
									resourcesManager.textures2D[resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.albedo->name].get();
		 }
		 if (entity.entities[i].signature & C_SIGNATURE::PHYSICS)
		 {
			 json physic = js["PhysicHandler"][indexOfPhysic];

			 LoadPhysic(physic, component.GetComponentPhysics(entity.entities[i].id));
			 
			 indexOfPhysic += 1;
		 }
		 if (entity.entities[i].signature & C_SIGNATURE::GAMEPLAY)
		 {
			 json gameplay = js["Gameplay"][indexOfGameplay];

			 LoadGameplay(gameplay, component.GetComponentGameplay(entity.entities[i].id), resourcesManager);

			 indexOfGameplay += 1;
		 }
	 }
 }

std::shared_ptr<Scene> Cookie::Resources::Serialization::Load::LoadScene(const char* filepath, Game& game)
 {
	 std::shared_ptr<Resources::Scene> newScene = std::make_shared<Resources::Scene>();
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
	 
	 /*
	 if (js.contains("Map"))
	 {
		 Cookie::Resources::Scene* scene = newScene.get();
		 Cookie::Resources::ResourcesManager resources = game.resources;

		 js["Map"]["tilesNb"].get_to(scene->map.tilesNb.e);
		 js["Map"]["tilesNbIcons"].get_to(scene->map.tilesNbIcons.e);
		 js["Map"]["tilesSize"].get_to(scene->map.tilesSize.e);

		 js["Map"]["trs"]["pos"].get_to(scene->map.trs.pos.e);
		 js["Map"]["trs"]["rot"].get_to(scene->map.trs.rot.e);
		 js["Map"]["trs"]["scale"].get_to(scene->map.trs.scale.e);

		 scene->map.model.mesh = resources.meshes[js["Map"]["model"]["mesh"].get<std::string>()];
		 scene->map.model.texture = resources.textures[js["Map"]["model"]["texture"].get<std::string>()];
		 scene->map.model.shader = resources.shaders[js["Map"]["model"]["shader"].get<std::string>()];

		 //LoadPhysic(js["Map"], scene->map.physic);
	 } */
	 
	 if (js.contains("EntityHandler"))
	 {
		 int newSizeEntities = js["EntityHandler"].size();
		 for (int i = newSizeEntities; i < newSizeEntities + newScene->entityHandler.livingEntities - newSizeEntities; i++)
		 {
			 if (newScene->entityHandler.entities[i].signature & C_SIGNATURE::TRANSFORM)
				 newScene->componentHandler.GetComponentTransform(newScene->entityHandler.entities[i].id).ToDefault();
			 if (newScene->entityHandler.entities[i].signature & C_SIGNATURE::MODEL)
				 newScene->componentHandler.GetComponentModel(newScene->entityHandler.entities[i].id).ToDefault();
			 if (newScene->entityHandler.entities[i].signature & C_SIGNATURE::PHYSICS)
				 newScene->componentHandler.GetComponentPhysics(newScene->entityHandler.entities[i].id).ToDefault();
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

	 for (unsigned int i = 0; i < filesPath.size(); i++)
	 {
		 std::string& iFile = filesPath.at(i);
		 std::replace(iFile.begin(), iFile.end(), '\\', '/');
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

		 if (js.contains("Model"))
		 {
			 if (js["Model"]["Mesh"].is_string())
				newPrefab.model.mesh = resourcesManager.meshes[js["Model"]["Mesh"]].get();
			 if (js["Model"]["Texture"].is_string())
				 newPrefab.model.albedo = resourcesManager.textures2D[js["Model"]["Texture"]].get();
		 }

		 if (js.contains("Transform"))
		 {
			 if (js["Transform"]["Rotation"].is_array())
				 js["Transform"]["Rotation"].get_to(newPrefab.transform.rot.e); 
			 if (js["Transform"]["Scale"].is_array())
				 js["Transform"]["Scale"].get_to(newPrefab.transform.scale.e);
		 }

		 if (js.contains("Signature"))
			 newPrefab.signature = js["Signature"];
		 
		 if (js.contains("physicTRS"))
		 {
			 json physic = js;

			 LoadPhysic(physic, newPrefab.physics);
		 }

		 if (js.contains("Gameplay"))
		 {
			 json gameplay = js["Gameplay"];

			 LoadGameplay(gameplay, newPrefab.gameplay, resourcesManager);
		 }

		 newPrefab.filepath = filesPath[i];

		 resourcesManager.prefabs[newPrefab.name] = std::make_unique<Prefab>(newPrefab);

		 file.close();
	 }
 }

void Cookie::Resources::Serialization::Load::LoadAllTextures(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 std::vector<std::string> filesPath;
	 for (const fs::directory_entry& path : fs::directory_iterator("Assets/Textures"))
	 {
		 if (path.path().string().find(".TAsset") != std::string::npos)
			 filesPath.push_back(path.path().string());
	 }

	 for (unsigned int i = 0; i < filesPath.size(); i++)
	 {
		 std::string& iFile = filesPath.at(i);
		 std::replace(iFile.begin(), iFile.end(), '\\', '/');
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

		 Cookie::Core::Math::Vec4 color;
		 std::string name;
		 js["color"].get_to(color.e);
		 js["name"].get_to(name);

		 resourcesManager.textures2D[name] = std::make_unique<Texture>(name, color);
	 }
 }

void Cookie::Resources::Serialization::Load::LoadPhysic(json& physic, Cookie::ECS::ComponentPhysics& physicsComp)
{
	json pTRS = physic["physicTRS"].at("position");
	json qTRS = physic["physicTRS"].at("quaternion");

	::reactphysics3d::Vector3 vecTemp(pTRS[0].get<float>(), pTRS[1].get<float>(), pTRS[1].get<float>());
	::reactphysics3d::Quaternion quatTemp(qTRS[1].get<float>(), qTRS[2].get<float>(), qTRS[3].get<float>(), qTRS[0].get<float>());
	physicsComp.physTransform = ::reactphysics3d::Transform({ vecTemp, quatTemp });

	physicsComp.physBody = Cookie::Physics::PhysicsHandle::physSim->createRigidBody(physicsComp.physTransform);

	//Rigidbody
	{
		json rigid = physic["Rigidbody"];
		::reactphysics3d::RigidBody* actRigidBody = physicsComp.physBody;

		if (rigid["type"].get<int>() == 0)
			actRigidBody->setType(::reactphysics3d::BodyType::STATIC);
		else if (rigid["type"].get<int>() == 1)
			actRigidBody->setType(::reactphysics3d::BodyType::KINEMATIC);
		else if (rigid["type"].get<int>() == 2)
			actRigidBody->setType(::reactphysics3d::BodyType::DYNAMIC);

		actRigidBody->setIsActive(rigid["active"].get<bool>());
		actRigidBody->setIsAllowedToSleep(rigid["allowedToSleep"].get<bool>());
		actRigidBody->setAngularDamping(rigid["angularDamping"].get<float>());
		actRigidBody->enableGravity(rigid["gravityEnabled"].get<bool>());
		actRigidBody->setLinearDamping(rigid["linearDamping"].get<float>());
		actRigidBody->setMass(rigid["mass"].get<float>());
		//check for sleeping 
	}


	for (int j = 0; j < physic["Colliders"].size(); j++)
	{
		json colliders = physic["Colliders"][j];

		Cookie::Core::Math::Vec3 pos;
		Cookie::Core::Math::Vec4 quat;
		colliders["transform"]["pos"].get_to(pos.e);
		colliders["transform"]["quaternion"].get_to(quat.e);
		Cookie::Core::Math::Vec3 rot = Cookie::Core::Math::Quat::ToEulerAngle({ quat.x, quat.y, quat.z, quat.w });

		if (colliders["type"].get<std::string>() == "Sphere")
		{
			float radius = colliders["radius"].get<float>();
			physicsComp.AddSphereCollider(radius, pos, rot);
		}
		else if (colliders["type"].get<std::string>() == "Capsule")
		{
			float radius = colliders["radius"].get<float>();
			float height = colliders["height"].get<float>();
			physicsComp.AddCapsuleCollider(Cookie::Core::Math::Vec2{ {radius, height } }, pos, rot);
		}
		else if (colliders["type"].get<std::string>() == "Box")
		{
			Cookie::Core::Math::Vec3 halfExtents;
			colliders["HalfExtents"].get_to(halfExtents.e);
			physicsComp.AddCubeCollider(halfExtents, pos, rot);
		}

		::reactphysics3d::Material& mat = physicsComp.physColliders[j]->getMaterial();
		mat.setBounciness(colliders["bounciness"].get<float>());
		mat.setMassDensity(colliders["density"].get<float>());
		mat.setFrictionCoefficient(colliders["frictionCoeff"].get<float>());
		mat.setRollingResistance(colliders["rollingResistance"].get<float>());
	}
}

void Cookie::Resources::Serialization::Load::LoadGameplay(json& gameplay, 
				Cookie::ECS::ComponentGameplay& GPComponent, Cookie::Resources::ResourcesManager& resourcesManager)
{
	GPComponent.teamName = gameplay["TeamName"];
	GPComponent.signatureGameplay = gameplay["SignatureGameplay"];
	GPComponent.type = gameplay["Type"];

	json temp = gameplay["Cost"];
	GPComponent.cost.costPrimary = temp["CostPrimary"].get<float>();
	GPComponent.cost.costSecondary = temp["CostSecondary"].get<float>();
	GPComponent.cost.costSupply = temp["CostSupply"].get<float>();
	GPComponent.cost.timeToProduce = temp["TimeToProduce"].get<float>();

	temp = gameplay["CGPLive"];
	GPComponent.componentLive.life = temp["Life"].get<float>();
	GPComponent.componentLive.armor = temp["Armor"].get<float>();

	temp = gameplay["CGPAttack"];
	GPComponent.componentAttack.needToAttack = temp["NeedToAttack"].get<bool>();
	GPComponent.componentAttack.attackDamage = temp["AttackDamage"].get<float>();
	GPComponent.componentAttack.attackSpeed = temp["AttackSpeed"].get<float>();
	GPComponent.componentAttack.attackRange = temp["AttackRange"].get<float>();

	temp = gameplay["CGPMove"];
	GPComponent.componentMove.moveSpeed = temp["MoveSpeed"].get<float>();
	GPComponent.componentMove.isFlying = temp["isFlying"].get<bool>();

	temp = gameplay["CGPProducer"];
	temp["TileSize"].get_to(GPComponent.componentProducer.tileSize.e);

	if (temp.contains("name"))
	{
		for (int i = 0; i < temp["name"].size(); i++)
		{
			std::string name = temp["name"][i].get<std::string>();
			if (resourcesManager.prefabs.find(name) != resourcesManager.prefabs.end())
				GPComponent.componentProducer.possibleUnits.push_back(resourcesManager.prefabs[name].get());
		}
	}

	if (gameplay.contains("CGPWorker"))
	{
		if (gameplay["CGPWorker"].contains("name"))
		{
			temp = gameplay["CGPWorker"]["name"];
			for (int i = 0; i < temp.size(); i++)
			{
				std::string name = temp[i].get<std::string>();
				if (resourcesManager.prefabs.find(name) != resourcesManager.prefabs.end())
					GPComponent.componentWorker.possibleBuildings.push_back(resourcesManager.prefabs[name].get());
			}
		}
	}
}

void Cookie::Resources::Serialization::Load::LoadVolumAndModeMusic(std::string path, std::string key)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::cout << "DON'T FIND THE FILE\n";
		return;
	}

	json js;
	file >> js;

	Cookie::Resources::SoundManager::SetVolume(key, js["Volume"].get<float>());

	Cookie::Core::Math::Vec3 pos;
	js["Pos"].get_to(pos.e);
	Cookie::Resources::SoundManager::SetPosition(key, pos);

	Cookie::Resources::SoundManager::SetMode(key, js["Mode"].get<int>());
}
