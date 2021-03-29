#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentPhysics.hpp"
#include "ComponentModel.hpp"
#include "ComponentScript.hpp"
#include "Debug.hpp"

#include <array>
#include <unordered_map>

#include "ECS/EntityHandler.hpp"
#include "Physics/PhysicsSimulator.hpp"


namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
	}

	namespace ECS
	{
		#define SIGNATURE_EMPTY         0b0000
		#define SIGNATURE_TRANSFORM     0b0001
		#define SIGNATURE_MODEL         0b0010
		#define SIGNATURE_PHYSICS		0b0100
		#define SIGNATURE_SCRIPT        0b1000
		#define SIGNATURE_ALL_COMPONENT 0b1111


		class ComponentHandler
		{
		public:

			std::array<ComponentTransform, MAX_ENTITIES> componentTransforms;

			std::array<ComponentModel, MAX_ENTITIES> componentModels;

			std::array<ComponentPhysics, MAX_ENTITIES> componentPhysics;

			std::array<ComponentScript, MAX_ENTITIES> componentScripts;



			ComponentHandler() {}
			~ComponentHandler() {}

			void AddComponentTransform(Entity& entity)
			{
				if (entity.signature & SIGNATURE_TRANSFORM)
				{
					CDebug.Warning("Component Transform already present");
					return;
				}

				entity.signature += SIGNATURE_TRANSFORM;
			}
			void AddComponentModel(Entity& entity)
			{
				if (entity.signature & SIGNATURE_MODEL)
				{
					CDebug.Warning("Component Model already present");
					return;
				}

				entity.signature += SIGNATURE_MODEL; 
			}
			void AddComponentPhysics(Entity& entity, const Physics::PhysicsSimulator& phs)
			{
				if (entity.signature & SIGNATURE_PHYSICS)
				{
					CDebug.Warning("Component Collider already present");
					return;
				}

				entity.signature += SIGNATURE_PHYSICS;

				componentTransforms [entity.id].SetPhysics();
				componentPhysics	[entity.id].physBody = phs.worldSim->createRigidBody(componentTransforms[entity.id].physTransform);
			}
			void AddComponentScript(Entity& entity)
			{
				if (entity.signature & SIGNATURE_SCRIPT)
				{
					CDebug.Warning("Component Script already present");
					return;
				}

				entity.signature += SIGNATURE_SCRIPT;
			}

			void ModifyComponentOfEntityToPrefab(Entity& entity, Cookie::Resources::ResourcesManager& resourcesManager, std::string& namePrefab);

			void RemoveComponentTransform(Entity& entity)
			{
				if (entity.signature & SIGNATURE_TRANSFORM)
				{
					GetComponentTransform(entity.id).ToDefault();
					entity.signature -= SIGNATURE_TRANSFORM;
					return;
				}
				
				CDebug.Warning("No Component Transform present");
			}
			void RemoveComponentModel(Entity& entity)
			{
				if (entity.signature & SIGNATURE_MODEL)
				{
					GetComponentModel(entity.id).ToDefault();
					entity.signature -= SIGNATURE_MODEL;
					return;
				}

				CDebug.Warning("No Component Model present");
			}
			void RemoveComponentPhysics(Entity& entity)
			{
				if (entity.signature & SIGNATURE_PHYSICS)
				{
					GetComponentPhysics(entity.id).ToDefault();
					entity.signature -= SIGNATURE_PHYSICS;
					return;
				}

				CDebug.Warning("No Component RigidBody present");
			}
			void RemoveComponentScript(Entity& entity)
			{
				if (entity.signature & SIGNATURE_SCRIPT)
				{
					GetComponentScript(entity.id).ToDefault();
					entity.signature -= SIGNATURE_SCRIPT;
					return;
				}

				CDebug.Warning("No Component Script present");
			}

			ComponentTransform& GetComponentTransform	(const unsigned int id) { return componentTransforms[id];	}
			ComponentModel&		GetComponentModel		(const unsigned int id) { return componentModels[id];		}
			ComponentPhysics&	GetComponentPhysics		(const unsigned int id) { return componentPhysics[id];		}
			ComponentScript&    GetComponentScript		(const unsigned int id) { return componentScripts[id];		}
		};

	}
}

#endif
