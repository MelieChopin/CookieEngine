#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentRigidBody.hpp"

#include <array>
#include <unordered_map>

namespace Cookie
{
	namespace ECS
	{
		#define SIGNATURE_EMPTY         0b000
		#define SIGNATURE_TRANSFORM     0b100
		#define SIGNATURE_RIGIDBODY     0b010
		#define SIGNATURE_MODEL         0b001
		#define SIGNATURE_ALL_COMPONENT 0b111


		class ComponentHandler
		{
		public:

			//Transform
			std::array<ComponentTransform, MAX_ENTITIES> componentTransforms;
			//std::unordered_map<unsigned int, unsigned int> transformEntityIdToIndex;
			//RigidBody
			std::array<ComponentRigidBody, MAX_ENTITIES> componentRigidBodies;
			//std::unordered_map<unsigned int, unsigned int> rigidBodyEntityIdToIndex;
			//Model
			std::array<ComponentModel, MAX_ENTITIES> componentModels;
			//std::unordered_map<unsigned int, unsigned int> modelEntityIdToIndex;


			ComponentHandler() {}
			~ComponentHandler() {}

			void AddComponentTransform(Entity& entity)
			{
				if (entity.signature & SIGNATURE_TRANSFORM)
				{
					std::cout << "Component Transform already present\n";
					return;
				}

				entity.signature += SIGNATURE_TRANSFORM;
			}
			void AddComponentRigidBody(Entity& entity)
			{
				if (entity.signature & SIGNATURE_RIGIDBODY)
				{
					std::cout << "Component RigidBody already present\n";
					return;
				}

				entity.signature += SIGNATURE_RIGIDBODY;
			}
			void AddComponentModel(Entity& entity)
			{
				if (entity.signature & SIGNATURE_MODEL)
				{
					std::cout << "Component Model already present\n";
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
				
				std::cout << "No Component Transform present\n";
			}
			void RemoveComponentRigidBody(Entity& entity)
			{
				if (entity.signature & SIGNATURE_RIGIDBODY)
				{
					GetComponentRigidBody(entity.id).ToDefault();
					entity.signature -= SIGNATURE_RIGIDBODY;
					return;
				}

				std::cout << "No Component RigidBody present\n";
			}
			void RemoveComponentModel(Entity& entity)
			{
				if (entity.signature & SIGNATURE_MODEL)
				{
					GetComponentModel(entity.id).ToDefault();
					entity.signature -= SIGNATURE_MODEL;
					return;
				}

				std::cout << "No Component Model present\n";
			}

			ComponentTransform& GetComponentTransform(const unsigned int id) { return componentTransforms[id]; }
			ComponentRigidBody& GetComponentRigidBody(const unsigned int id) { return componentRigidBodies[id]; }
			ComponentModel&		GetComponentModel    (const unsigned int id) { return componentModels[id]; }


		};

	}
}

#endif