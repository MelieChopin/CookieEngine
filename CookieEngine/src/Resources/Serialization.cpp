#include <iostream>
#include <fstream>
#include <iomanip>
#include "Physics/PhysicsHandle.hpp"
#include "Coordinator.hpp"
#include "Resources/Scene.hpp"
#include "Game.hpp"
#include "Resources/Serialization.hpp"
#include "Resources/Prefab.hpp"
#include "ComponentPhysics.hpp"
#include <bitset>
#include <filesystem>
#include <reactphysics3d/reactphysics3d.h>

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
		if (entity.entities[i].signature & SIGNATURE_PHYSICS)
		{
			std::vector<::reactphysics3d::Collider*> colliders = component.GetComponentPhysics(entity.entities[i].id).physColliders;
			int index = js["PhysicHandler"].size();
			for (int j = 0; j < colliders.size(); j++)
			{
				::reactphysics3d::Collider* actCollider = colliders[j];
				if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::SPHERE)
				{
					float radius = static_cast<::reactphysics3d::SphereShape*>(actCollider->getCollisionShape())->getRadius();
					js["PhysicHandler"][index]["Colliders"][j]["type"] = "Sphere";
					js["PhysicHandler"][index]["Colliders"][j]["radius"] = radius;
				}
				else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::CAPSULE)
				{
					::reactphysics3d::CapsuleShape* capsule = static_cast<::reactphysics3d::CapsuleShape*>(actCollider->getCollisionShape());
					js["PhysicHandler"][index]["Colliders"][j]["type"] = "Capsule";
					js["PhysicHandler"][index]["Colliders"][j]["radius"] = capsule->getRadius();
					js["PhysicHandler"][index]["Colliders"][j]["height"] = capsule->getHeight();
				}
				else if (actCollider->getCollisionShape()->getName() == ::reactphysics3d::CollisionShapeName::BOX)
				{
					::reactphysics3d::Vector3 box = static_cast<::reactphysics3d::BoxShape*>(actCollider->getCollisionShape())->getHalfExtents();
					js["PhysicHandler"][index]["Colliders"][j]["type"] = "Box";
					js["PhysicHandler"][index]["Colliders"][j]["HalfExtents"] = { box.x, box.y, box.z };
				}

				::reactphysics3d::Material& mat = actCollider->getMaterial();

				::reactphysics3d::Vector3		vec = actCollider->getLocalToBodyTransform().getPosition();
				::reactphysics3d::Quaternion	quat = actCollider->getLocalToBodyTransform().getOrientation();
				js["PhysicHandler"][index]["Colliders"][j]["transform"]["pos"] = { vec.x, vec.y, vec.z };
				js["PhysicHandler"][index]["Colliders"][j]["transform"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
				js["PhysicHandler"][index]["Colliders"][j]["bounciness"] = mat.getBounciness();
				js["PhysicHandler"][index]["Colliders"][j]["frictionCoeff"] = mat.getFrictionCoefficient();
				js["PhysicHandler"][index]["Colliders"][j]["density"] = mat.getMassDensity();
				js["PhysicHandler"][index]["Colliders"][j]["rollingResistance"] = mat.getRollingResistance();
			}

			::reactphysics3d::RigidBody*& rigibody = component.GetComponentPhysics(entity.entities[i].id).physBody;

			
			js["PhysicHandler"][index]["Rigidbody"]["type"] = rigibody->getType();
			js["PhysicHandler"][index]["Rigidbody"]["angularDamping"] = rigibody->getAngularDamping();
			js["PhysicHandler"][index]["Rigidbody"]["linearDamping"] = rigibody->getLinearDamping();
			js["PhysicHandler"][index]["Rigidbody"]["mass"] = rigibody->getMass();
			js["PhysicHandler"][index]["Rigidbody"]["active"] = rigibody->isActive();
			js["PhysicHandler"][index]["Rigidbody"]["allowedToSleep"] = rigibody->isAllowedToSleep();
			js["PhysicHandler"][index]["Rigidbody"]["sleeping"] = rigibody->isSleeping();
			js["PhysicHandler"][index]["Rigidbody"]["gravityEnabled"] = rigibody->isGravityEnabled();

			::reactphysics3d::Transform physTrans = component.GetComponentPhysics(entity.entities[i].id).physTransform;
			::reactphysics3d::Vector3 pos = physTrans.getPosition();
			::reactphysics3d::Quaternion quat = physTrans.getOrientation();

			js["PhysicHandler"][index]["physicTRS"]["position"] = { pos.x, pos.y, pos.z };
			js["PhysicHandler"][index]["physicTRS"]["quaternion"] = { quat.w, quat.x, quat.y, quat.z };
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

	 if (prefab->rigidBody != nullptr)
	 {
		 js["Rigidbody"]["type"] = prefab->rigidBody->getType();
		 js["Rigidbody"]["angularDamping"] = prefab->rigidBody->getAngularDamping();
		 js["Rigidbody"]["linearDamping"] = prefab->rigidBody->getLinearDamping();
		 js["Rigidbody"]["mass"] = prefab->rigidBody->getMass();
		 js["Rigidbody"]["active"] = prefab->rigidBody->isActive();
		 js["Rigidbody"]["allowedToSleep"] = prefab->rigidBody->isAllowedToSleep();
		 js["Rigidbody"]["sleeping"] = prefab->rigidBody->isSleeping();
		 js["Rigidbody"]["gravityEnabled"] = prefab->rigidBody->isGravityEnabled();
	 }
	 else
		 js["Rigidbody"] = "nullptr";

	 if (prefab->colliders.size() != 0)
	 {
		 for (int i = 0; i < prefab->colliders.size(); i++)
		 {
			 ::reactphysics3d::Collider* actCollider = prefab->colliders[i];
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
	 else
		 js["Colliders"] = "nullptr";

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
			 json physic = js["PhysicHandler"][indexOfPhysic];
			 json pTRS = physic["physicTRS"].at("position");
			 json qTRS = physic["physicTRS"].at("quaternion");

			 ::reactphysics3d::Vector3 vecTemp(pTRS[0].get<float>(), pTRS[1].get<float>(), pTRS[1].get<float>());
			 ::reactphysics3d::Quaternion quatTemp(qTRS[1].get<float>(), qTRS[2].get<float>(), qTRS[3].get<float>(), qTRS[0].get<float>());
			 component.componentPhysics[entity.entities[i].id].physTransform = ::reactphysics3d::Transform({ vecTemp, quatTemp });

			 component.componentPhysics[entity.entities[i].id].physBody = 
				 Physics::PhysicsHandle::physSim->createRigidBody(component.componentPhysics[entity.entities[i].id].physTransform);

			 //Rigidbody
			 {
				 json rigid = physic["Rigidbody"];
				 ::reactphysics3d::RigidBody* actRigidBody = component.componentPhysics[entity.entities[i].id].physBody;

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
					 component.componentPhysics[entity.entities[i].id].AddSphereCollider(radius, pos, rot);
				 }
				 else if (colliders["type"].get<std::string>() == "Capsule")
				 {
					 float radius = colliders["radius"].get<float>();
					 float height = colliders["height"].get<float>();
					 component.componentPhysics[entity.entities[i].id].AddCapsuleCollider(Cookie::Core::Math::Vec2{ radius, height }, pos, rot);
				 }
				 else if (colliders["type"].get<std::string>() == "Box")
				 {
					 Cookie::Core::Math::Vec3 halfExtents;
					 colliders["HalfExtents"].get_to(halfExtents.e);
					 component.GetComponentPhysics(entity.entities[i].id).AddCubeCollider(halfExtents, pos, rot);
				 }
				 
				 ::reactphysics3d::Material& mat = component.componentPhysics[entity.entities[i].id].physColliders[j]->getMaterial();
				 mat.setBounciness(colliders["bounciness"].get<float>());
				 mat.setMassDensity(colliders["density"].get<float>());
				 mat.setFrictionCoefficient(colliders["frictionCoeff"].get<float>());
				 mat.setRollingResistance(colliders["rollingResistance"].get<float>());
			 }

			 indexOfPhysic += 1;
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
			 if (newScene->entityHandler.entities[i].signature & SIGNATURE_PHYSICS)
				 newScene->componentHandler.componentPhysics[newScene->entityHandler.entities[i].id].ToDefault();
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


 void Cookie::Resources::Serialization::Load::LoadAllTextures(Cookie::Resources::ResourcesManager& resourcesManager)
 {
	 std::vector<std::string> filesPath;
	 for (const fs::directory_entry& path : fs::directory_iterator("Assets/Textures"))
	 {
		 if (path.path().string().find(".TAsset") != std::string::npos)
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

		 Cookie::Core::Math::Vec4 color;
		 std::string name;
		 js["color"].get_to(color.e);
		 js["name"].get_to(name);

		 resourcesManager.textures[name] = std::make_shared<Texture>(name, color);
	 }
 }