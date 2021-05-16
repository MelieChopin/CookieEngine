
#include "Physics/PhysicsHandle.hpp"
#include "Game.hpp"

#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Scene.hpp"

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

			if (entity.entities[i].namePrefab != "NONE")
			{
				js["ComponentHandler"]["Transform"] += json{ { "localTRS", { { "translate", transform.pos.e } } } };
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.rot != transform.rot)
					js["ComponentHandler"]["Transform"].at(js["ComponentHandler"]["Transform"].size() - 1).at("localTRS")["rotation"] = transform.rot.e;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.scale != transform.scale)
					js["ComponentHandler"]["Transform"].at(js["ComponentHandler"]["Transform"].size() - 1).at("localTRS")["scale"] = transform.scale.e;
			}
			else
			{
				js["ComponentHandler"]["Transform"] += json{ { "localTRS", { { "translate", transform.pos.e },
													{ "rotation", transform.rot.e },
													{ "scale", transform.scale.e } } } };
			}
		}
		if (entity.entities[i].signature & C_SIGNATURE::MODEL)
		{
			Cookie::ECS::ComponentModel model = component.GetComponentModel(entity.entities[i].id);
			if (entity.entities[i].namePrefab != "NONE")
			{
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.mesh->name != model.mesh->name)
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size()]["model"] = model.mesh->name;
				else
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size()]["model"] = 0;
				if (resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.albedo->name != model.albedo->name)
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size() - 1]["texture"] = model.albedo->name;
				else
					js["ComponentHandler"]["Model"][js["ComponentHandler"]["Model"].size() - 1]["texture"] = 0;
			}
			else
			{
				js["ComponentHandler"]["Model"] += json{ { "model", model.mesh != nullptr ? model.mesh->name : "NO MESH" },
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
			js["Gameplay"][index]["TeamName"] = gameplay.teamName;
			js["Gameplay"][index]["SignatureGameplay"] = gameplay.signatureGameplay;
			js["Gameplay"][index]["Cost"]["CostPrimary"] = gameplay.cost.costPrimary;
			js["Gameplay"][index]["Cost"]["CostSecondary"] = gameplay.cost.costSecondary;
			js["Gameplay"][index]["Cost"]["CostSupply"] = gameplay.cost.costSupply;
			js["Gameplay"][index]["Cost"]["TimeToProduce"] = gameplay.cost.timeToProduce;

			js["Gameplay"][index]["CGPLive"]["Life"] = gameplay.componentLive.life;
			js["Gameplay"][index]["CGPLive"]["Armor"] = gameplay.componentLive.armor;

			js["Gameplay"][index]["CGPAttack"]["NeedToAttack"] = gameplay.componentAttack.needToAttack;
			js["Gameplay"][index]["CGPAttack"]["AttackDamage"] = gameplay.componentAttack.attackDamage;
			js["Gameplay"][index]["CGPAttack"]["AttackSpeed"] = gameplay.componentAttack.attackSpeed;
			js["Gameplay"][index]["CGPAttack"]["AttackCooldown"] = gameplay.componentAttack.attackCooldown;
			js["Gameplay"][index]["CGPAttack"]["AttackRange"] = gameplay.componentAttack.attackRange;

			js["Gameplay"][index]["CGPMove"]["MoveSpeed"] = gameplay.componentMove.moveSpeed;
			js["Gameplay"][index]["CGPMove"]["isFlying"] = gameplay.componentMove.isFlying;

			for (int i = 0; i < gameplay.componentProducer.possibleUnits.size(); i++)
				js["Gameplay"][index]["CGPProducer"]["name"] += gameplay.componentProducer.possibleUnits[i]->name;

			js["Gameplay"][index]["CGPProducer"]["TileSize"] = gameplay.componentProducer.tileSize.e;

			for (int i = 0; i < gameplay.componentWorker.possibleBuildings.size(); i++)
				js["Gameplay"][index]["CGPWorker"]["name"] += gameplay.componentWorker.possibleBuildings[i]->name;
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

void Cookie::Resources::Serialization::Save::SavePrefab(const std::shared_ptr<Prefab>& prefab)
 {
	 std::ofstream file(prefab->filepath);

	 json js;

	 Prefab* pref = prefab.get();

	 js["Signature"] = pref->signature;
	 js["Name"] = pref->name;
	 
	 if (pref->signature & C_SIGNATURE::TRANSFORM)
	 {
		 //Transform
		 js["Transform"]["Rotation"] = pref->transform.rot.e;
		 js["Transform"]["Scale"] = pref->transform.scale.e;
	 }
	 
	 if (pref->signature & C_SIGNATURE::MODEL)
	 {
		 //Model
		 if (pref->model.mesh != nullptr)
			js["Model"]["Mesh"] = pref->model.mesh->name;
		 else
			js["Model"]["Mesh"] = 0;
		 if (pref->model.albedo != nullptr)
			js["Model"]["Texture"] = pref->model.albedo->name;
		 else
			js["Model"]["Texture"] = 0;
	 }

	 if (pref->signature & C_SIGNATURE::PHYSICS)
	 {
		 if (pref->physics.physBody != nullptr)
		 {
			 js["Rigidbody"]["type"] = pref->physics.physBody->getType();
			 js["Rigidbody"]["angularDamping"] = pref->physics.physBody->getAngularDamping();
			 js["Rigidbody"]["linearDamping"] = pref->physics.physBody->getLinearDamping();
			 js["Rigidbody"]["mass"] = pref->physics.physBody->getMass();
			 js["Rigidbody"]["active"] = pref->physics.physBody->isActive();
			 js["Rigidbody"]["allowedToSleep"] = pref->physics.physBody->isAllowedToSleep();
			 js["Rigidbody"]["sleeping"] = pref->physics.physBody->isSleeping();
			 js["Rigidbody"]["gravityEnabled"] = pref->physics.physBody->isGravityEnabled();
		 }
	 }
	 else
		 js["Rigidbody"] = "nullptr";

	 if (pref->signature & C_SIGNATURE::PHYSICS)
	 {
		 if (prefab->physics.physColliders.size() != 0)
		 {
			 for (int i = 0; i < prefab->physics.physColliders.size(); i++)
			 {
				 ::reactphysics3d::Collider* actCollider = prefab->physics.physColliders[i];
				 if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::SPHERE)
				 {
					 float radius = static_cast<::reactphysics3d::SphereShape*>(actCollider->getCollisionShape())->getRadius();
					 js["Colliders"][i]["type"] = "Sphere";
					 js["Colliders"][i]["radius"] = radius;
				 }
				 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::CAPSULE)
				 {
					 ::reactphysics3d::CapsuleShape* capsule = static_cast<::reactphysics3d::CapsuleShape*>(actCollider->getCollisionShape());
					 js["Colliders"][i]["type"] = "Capsule";
					 js["Colliders"][i]["radius"] = capsule->getRadius();
					 js["Colliders"][i]["height"] = capsule->getHeight();
				 }
				 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::BOX)
				 {
					 ::reactphysics3d::Vector3 box = static_cast<::reactphysics3d::BoxShape*>(actCollider->getCollisionShape())->getHalfExtents();
					 js["Colliders"][i]["type"] = "Box";
					 js["Colliders"][i]["HalfExtents"] = { box.x, box.y, box.z };
				 }

				 ::reactphysics3d::Material& mat = actCollider->getMaterial();

				 ::reactphysics3d::Vector3		vec = actCollider->getLocalToBodyTransform().getPosition();
				 ::reactphysics3d::Quaternion	quat = actCollider->getLocalToBodyTransform().getOrientation();
				 js["Colliders"][i]["transform"]["pos"] = { vec.x, vec.y, vec.z };
				 js["Colliders"][i]["transform"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
				 js["Colliders"][i]["bounciness"] = mat.getBounciness();
				 js["Colliders"][i]["frictionCoeff"] = mat.getFrictionCoefficient();
				 js["Colliders"][i]["density"] = mat.getMassDensity();
				 js["Colliders"][i]["rollingResistance"] = mat.getRollingResistance();
			 }
		 }
	 }
	 else
		 js["Colliders"] = "nullptr";

	 if (pref->signature & C_SIGNATURE::GAMEPLAY)
	 {
		 ComponentGameplay gameplay = prefab->gameplay;
		 js["Gameplay"]["TeamName"] = gameplay.teamName;
		 js["Gameplay"]["SignatureGameplay"] = gameplay.signatureGameplay;
		 js["Gameplay"]["Cost"]["CostPrimary"] = gameplay.cost.costPrimary;
		 js["Gameplay"]["Cost"]["CostSecondary"] = gameplay.cost.costSecondary;
		 js["Gameplay"]["Cost"]["CostSupply"] = gameplay.cost.costSupply;
		 js["Gameplay"]["Cost"]["TimeToProduce"] = gameplay.cost.timeToProduce;

		 js["Gameplay"]["CGPLive"]["Life"] = gameplay.componentLive.life;
		 js["Gameplay"]["CGPLive"]["Armor"] = gameplay.componentLive.armor;

		 js["Gameplay"]["CGPAttack"]["NeedToAttack"] = gameplay.componentAttack.needToAttack;
		 js["Gameplay"]["CGPAttack"]["AttackDamage"] = gameplay.componentAttack.attackDamage;
		 js["Gameplay"]["CGPAttack"]["AttackSpeed"] = gameplay.componentAttack.attackSpeed;
		 js["Gameplay"]["CGPAttack"]["AttackCooldown"] = gameplay.componentAttack.attackCooldown;
		 js["Gameplay"]["CGPAttack"]["AttackRange"] = gameplay.componentAttack.attackRange;

		 js["Gameplay"]["CGPMove"]["MoveSpeed"] = gameplay.componentMove.moveSpeed;
		 js["Gameplay"]["CGPMove"]["isFlying"] = gameplay.componentMove.isFlying;

		 for (int i = 0; i < gameplay.componentProducer.possibleUnits.size(); i++)
			 js["Gameplay"]["CGPProducer"]["name"] += gameplay.componentProducer.possibleUnits[i]->name;

		 js["Gameplay"]["CGPProducer"]["TileSize"] = gameplay.componentProducer.tileSize.e;

		 for (int i = 0; i < gameplay.componentWorker.possibleBuildings.size(); i++)
			 js["Gameplay"]["CGPWorker"]["name"] += gameplay.componentWorker.possibleBuildings[i]->name;
	 }

	 file << std::setw(4) << js << std::endl;
 }

void Cookie::Resources::Serialization::Save::SaveAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 for (std::unordered_map<std::string, std::shared_ptr<Prefab>>::iterator prefab = resourcesManager.prefabs.begin(); prefab != resourcesManager.prefabs.end(); prefab++)
		 Resources::Serialization::Save::SavePrefab(prefab->second);
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
		 if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::SPHERE)
		 {
			 float radius = static_cast<::reactphysics3d::SphereShape*>(actCollider->getCollisionShape())->getRadius();
			 js["Colliders"][j]["type"] = "Sphere";
			 js["Colliders"][j]["radius"] = radius;
		 }
		 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::CAPSULE)
		 {
			 ::reactphysics3d::CapsuleShape* capsule = static_cast<::reactphysics3d::CapsuleShape*>(actCollider->getCollisionShape());
			 js["Colliders"][j]["type"] = "Capsule";
			 js["Colliders"][j]["radius"] = capsule->getRadius();
			 js["Colliders"][j]["height"] = capsule->getHeight();
		 }
		 else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::BOX)
		 {
			 ::reactphysics3d::Vector3 box = static_cast<::reactphysics3d::BoxShape*>(actCollider->getCollisionShape())->getHalfExtents();
			 js["Colliders"][j]["type"] = "Box";
			 js["Colliders"][j]["HalfExtents"] = { box.x, box.y, box.z };
		 }

		 ::reactphysics3d::Material& mat = actCollider->getMaterial();

		 ::reactphysics3d::Vector3		vec = actCollider->getLocalToBodyTransform().getPosition();
		 ::reactphysics3d::Quaternion	quat = actCollider->getLocalToBodyTransform().getOrientation();
		 js["Colliders"][j]["transform"]["pos"] = { vec.x, vec.y, vec.z };
		 js["Colliders"][j]["transform"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
		 js["Colliders"][j]["bounciness"] = mat.getBounciness();
		 js["Colliders"][j]["frictionCoeff"] = mat.getFrictionCoefficient();
		 js["Colliders"][j]["density"] = mat.getMassDensity();
		 js["Colliders"][j]["rollingResistance"] = mat.getRollingResistance();
	 }

	 ::reactphysics3d::RigidBody*& rigibody = physics.physBody;


	 js["Rigidbody"]["type"] = rigibody->getType();
	 js["Rigidbody"]["angularDamping"] = rigibody->getAngularDamping();
	 js["Rigidbody"]["linearDamping"] = rigibody->getLinearDamping();
	 js["Rigidbody"]["mass"] = rigibody->getMass();
	 js["Rigidbody"]["active"] = rigibody->isActive();
	 js["Rigidbody"]["allowedToSleep"] = rigibody->isAllowedToSleep();
	 js["Rigidbody"]["sleeping"] = rigibody->isSleeping();
	 js["Rigidbody"]["gravityEnabled"] = rigibody->isGravityEnabled();

	 ::reactphysics3d::Transform physTrans = physics.physTransform;
	 ::reactphysics3d::Vector3 pos = physTrans.getPosition();
	 ::reactphysics3d::Quaternion quat = physTrans.getOrientation();

	 js["physicTRS"]["position"] = { pos.x, pos.y, pos.z };
	 js["physicTRS"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
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
				 transform.scale = resourcesManager.prefabs[entity.entities[i].namePrefab].get()->transform.scale;
			 
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
				component.GetComponentModel(entity.entities[i].id).albedo = resourcesManager.textures[(model.at("texture").get<std::string>())].get();
			 else if (entity.entities[i].namePrefab != "NONE")
				 component.GetComponentModel(entity.entities[i].id).albedo =
									resourcesManager.textures[resourcesManager.prefabs[entity.entities[i].namePrefab].get()->model.albedo->name].get();
		 }
		 if (entity.entities[i].signature & C_SIGNATURE::PHYSICS)
		 {
			 json physic = js["PhysicHandler"][indexOfPhysic];

			 LoadPhysic(physic, component.GetComponentPhysics(entity.entities[i].id));
			 
			 indexOfPhysic += 1;
		 }
		 if (entity.entities[i].signature & C_SIGNATURE::GAMEPLAY)
		 {
			 ComponentGameplay GPComponent;
			 json gameplay = js["Gameplay"][indexOfGameplay];

			 GPComponent.teamName = gameplay["TeamName"];
			 GPComponent.signatureGameplay = gameplay["SignatureGameplay"];
			
			 GPComponent.cost.costPrimary = gameplay["Cost"]["CostPrimary"].get<float>();
			 GPComponent.cost.costSecondary = gameplay["Cost"]["CostSecondary"].get<float>();
			 GPComponent.cost.costSupply = gameplay["Cost"]["CostSupply"].get<float>();
			 GPComponent.cost.timeToProduce = gameplay["Cost"]["TimeToProduce"].get<float>();

			 GPComponent.componentLive.life = gameplay["CGPLive"]["Life"].get<float>();
			 GPComponent.componentLive.armor = gameplay["CGPLive"]["Armor"].get<float>();

			 GPComponent.componentAttack.needToAttack = gameplay["CGPAttack"]["NeedToAttack"].get<bool>();
			 GPComponent.componentAttack.attackDamage = gameplay["CGPAttack"]["AttackDamage"].get<float>();
			 GPComponent.componentAttack.attackSpeed = gameplay["CGPAttack"]["AttackSpeed"].get<float>();
			 GPComponent.componentAttack.attackCooldown = gameplay["CGPAttack"]["AttackCooldown"].get<float>();
			 GPComponent.componentAttack.attackRange = gameplay["CGPAttack"]["AttackRange"].get<float>();

			 GPComponent.componentMove.moveSpeed = gameplay["CGPMove"]["MoveSpeed"].get<float>();
			 GPComponent.componentMove.isFlying = gameplay["CGPMove"]["isFlying"].get<bool>();

			 gameplay["CGPProducer"]["TileSize"].get_to(GPComponent.componentProducer.tileSize.e);

			 if (gameplay["CGPProducer"].contains("name"))
			 {
				 for (int i = 0; i < gameplay["CGPProducer"]["name"].size(); i++)
				 {
					 std::string name = gameplay["CGPProducer"]["name"][i].get<std::string>();
					 if (resourcesManager.prefabs.find(name) != resourcesManager.prefabs.end())
						GPComponent.componentProducer.possibleUnits.push_back(resourcesManager.prefabs[name].get());
				 }
			 }

			 if (gameplay.contains("CGPWorker"))
			 {
				 if (gameplay["CGPWorker"].contains("name"))
				 {
					 for (int i = 0; i < gameplay["CGPWorker"]["name"].size(); i++)
					 {
						 std::string name = gameplay["CGPWorker"]["name"][i].get<std::string>();
						 if (resourcesManager.prefabs.find(name) != resourcesManager.prefabs.end())
							 GPComponent.componentWorker.possibleBuildings.push_back(resourcesManager.prefabs[name].get());
					 }
				 }
			 }

			 component.GetComponentGameplay(entity.entities[i].id) = GPComponent;
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
				 newPrefab.model.albedo = resourcesManager.textures[js["Model"]["Texture"]].get();
		 }

		 if (js.contains("Transform"))
		 {
			 if (js["Transform"]["Rotation"].is_array())
				 js["Transform"]["Rotation"].get_to(newPrefab.transform.rot.e); 
			 if (js["Transform"]["Scale"].is_string())
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
			 ComponentGameplay GPComponent;
			 json gameplay = js["Gameplay"];

			 GPComponent.teamName = gameplay["TeamName"];
			 GPComponent.signatureGameplay = gameplay["SignatureGameplay"];

			 GPComponent.cost.costPrimary = gameplay["Cost"]["CostPrimary"].get<float>();
			 GPComponent.cost.costSecondary = gameplay["Cost"]["CostSecondary"].get<float>();
			 GPComponent.cost.costSupply = gameplay["Cost"]["CostSupply"].get<float>();
			 GPComponent.cost.timeToProduce = gameplay["Cost"]["TimeToProduce"].get<float>();

			 GPComponent.componentLive.life = gameplay["CGPLive"]["Life"].get<float>();
			 GPComponent.componentLive.armor = gameplay["CGPLive"]["Armor"].get<float>();

			 GPComponent.componentAttack.needToAttack = gameplay["CGPAttack"]["NeedToAttack"].get<bool>();
			 GPComponent.componentAttack.attackDamage = gameplay["CGPAttack"]["AttackDamage"].get<float>();
			 GPComponent.componentAttack.attackSpeed = gameplay["CGPAttack"]["AttackSpeed"].get<float>();
			 GPComponent.componentAttack.attackCooldown = gameplay["CGPAttack"]["AttackCooldown"].get<float>();
			 GPComponent.componentAttack.attackRange = gameplay["CGPAttack"]["AttackRange"].get<float>();

			 GPComponent.componentMove.moveSpeed = gameplay["CGPMove"]["MoveSpeed"].get<float>();
			 GPComponent.componentMove.isFlying = gameplay["CGPMove"]["isFlying"].get<bool>();

			 gameplay["CGPProducer"]["TileSize"].get_to(GPComponent.componentProducer.tileSize.e);

			 if (gameplay["CGPProducer"].contains("name"))
			 {
				 for (int i = 0; i < gameplay["CGPProducer"]["name"].size(); i++)
				 {
					 std::string name = gameplay["CGPProducer"]["name"][i].get<std::string>();
					 if (resourcesManager.prefabs.find(name) != resourcesManager.prefabs.end())
						 GPComponent.componentProducer.possibleUnits.push_back(resourcesManager.prefabs[name].get());
				 }
			 }

			 if (gameplay.contains("CGPWorker"))
			 {
				 if (gameplay["CGPWorker"].contains("name"))
				 {
					 for (int i = 0; i < gameplay["CGPWorker"]["name"].size(); i++)
					 {
						 std::string name = gameplay["CGPWorker"]["name"][i].get<std::string>();
						 if (resourcesManager.prefabs.find(name) != resourcesManager.prefabs.end())
							 GPComponent.componentWorker.possibleBuildings.push_back(resourcesManager.prefabs[name].get());
					 }
				 }
			 }
			 newPrefab.gameplay = GPComponent;
		 }

		 newPrefab.filepath = filesPath[i];

		 resourcesManager.prefabs[newPrefab.name] = std::make_shared<Prefab>(newPrefab);

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

		 resourcesManager.textures[name] = std::make_unique<Texture>(name, color);
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
