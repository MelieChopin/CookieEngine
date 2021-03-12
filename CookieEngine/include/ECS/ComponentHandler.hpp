#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentRigidBody.hpp"
#include "ComponentModel.hpp"
#include "ComponentScript.hpp"
#include "Debug.hpp"

#include <array>
#include <unordered_map>

namespace Cookie
{
	namespace ECS
	{
		#define SIGNATURE_EMPTY         0b0000
		#define SIGNATURE_TRANSFORM     0b1000
		#define SIGNATURE_RIGIDBODY     0b0100
		#define SIGNATURE_MODEL         0b0010
		#define SIGNATURE_SCRIPT        0b0001
		#define SIGNATURE_ALL_COMPONENT 0b1111


		class ComponentHandler
		{
		public:

			std::array<ComponentTransform, MAX_ENTITIES> componentTransforms;

			std::array<ComponentRigidBody, MAX_ENTITIES> componentRigidBodies;

			std::array<ComponentModel, MAX_ENTITIES> componentModels;

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
			void AddComponentRigidBody(Entity& entity)
			{
				if (entity.signature & SIGNATURE_RIGIDBODY)
				{
					CDebug.Warning("Component RigidBody already present");
					return;
				}

				entity.signature += SIGNATURE_RIGIDBODY;
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
			void AddComponentScript(Entity& entity)
			{
				if (entity.signature & SIGNATURE_SCRIPT)
				{
					CDebug.Warning("Component Script already present");
					return;
				}

				entity.signature += SIGNATURE_SCRIPT;
			}

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
			void RemoveComponentRigidBody(Entity& entity)
			{
				if (entity.signature & SIGNATURE_RIGIDBODY)
				{
					GetComponentRigidBody(entity.id).ToDefault();
					entity.signature -= SIGNATURE_RIGIDBODY;
					return;
				}

				CDebug.Warning("No Component RigidBody present");
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

			ComponentTransform& GetComponentTransform(const unsigned int id) { return componentTransforms[id]; }
			ComponentRigidBody& GetComponentRigidBody(const unsigned int id) { return componentRigidBodies[id]; }
			ComponentModel&		GetComponentModel    (const unsigned int id) { return componentModels[id]; }
			ComponentScript&    GetComponentScript   (const unsigned int id) { return componentScripts[id]; }


		};

	}
}

#endif