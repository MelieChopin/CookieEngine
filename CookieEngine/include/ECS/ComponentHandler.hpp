#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentCollider.hpp"
#include "ComponentRigidBody.hpp"
#include "Debug.hpp"

#include <array>
#include <unordered_map>

namespace Cookie
{
	namespace ECS
	{
		#define SIGNATURE_EMPTY         0b0000
		#define SIGNATURE_COLLIDER		0b1000
		#define SIGNATURE_RIGIDBODY     0b0100
		#define SIGNATURE_TRANSFORM     0b0010
		#define SIGNATURE_MODEL         0b0001
		#define SIGNATURE_ALL_COMPONENT 0b1111


		class ComponentHandler
		{
		public:

			std::array<ComponentTransform, MAX_ENTITIES> componentTransforms;

			std::array<ComponentRigidBody, MAX_ENTITIES> componentRigidBodies;

			std::array<ComponentCollider, MAX_ENTITIES> componentColliders;

			std::array<ComponentModel, MAX_ENTITIES> componentModels;



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
			void AddComponentCollider(Entity& entity)
			{
				if (entity.signature & SIGNATURE_COLLIDER)
				{
					CDebug.Warning("Component Collider already present");
					return;
				}

				entity.signature += SIGNATURE_COLLIDER;
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
			void RemoveComponentCollider(Entity& entity)
			{
				if (entity.signature & SIGNATURE_COLLIDER)
				{
					GetComponentCollider(entity.id).ToDefault();
					entity.signature -= SIGNATURE_COLLIDER;
					return;
				}

				CDebug.Warning("No Component RigidBody present");
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

			ComponentTransform& GetComponentTransform(const unsigned int id) { return componentTransforms[id]; }
			ComponentRigidBody& GetComponentRigidBody(const unsigned int id) { return componentRigidBodies[id]; }
			ComponentModel&		GetComponentModel    (const unsigned int id) { return componentModels[id]; }
			ComponentCollider&	GetComponentCollider (const unsigned int id) { return componentColliders[id]; }


		};

	}
}

#endif