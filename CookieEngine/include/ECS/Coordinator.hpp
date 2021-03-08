#ifndef __COORDINATOR_HPP__
#define __COORDINATOR_HPP__

#include "EntityHandler.hpp"
#include "ComponentHandler.hpp"
#include "SystemHandler.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Render/Camera.hpp"
#include <assert.h>

namespace Cookie
{
	namespace ECS
	{

		class Coordinator
		{
		public:
			EntityHandler*     entityHandler    {nullptr};
			ComponentHandler*  componentHandler {nullptr};
			Entity*            selectedEntity   {nullptr};

			Coordinator() {}
			~Coordinator() {}

			void AddEntity(const int signature, Resources::ResourcesManager	resources, std::string name = std::string("No Name"))
			{
				assert(entityHandler->livingEntities < MAX_ENTITIES && "Too many entities in existence.");

				entityHandler->entities[entityHandler->livingEntities].name = name;
				unsigned int id = entityHandler->entities[entityHandler->livingEntities].id;
				entityHandler->livingEntities++;

				if (CheckSignature(signature, SIGNATURE_TRANSFORM))
					componentHandler->AddComponentTransform(entityHandler->entities[id]);
				if (CheckSignature(signature, SIGNATURE_RIGIDBODY))
					componentHandler->AddComponentRigidBody(entityHandler->entities[id]);
				if (CheckSignature(signature, SIGNATURE_MODEL))
					componentHandler->AddComponentModel(entityHandler->entities[id]);

				//not clean should be moved somewhere else
				componentHandler->GetComponentModel(id).shader = resources.GetDefaultShader();
			}
			static void AddEntity(EntityHandler& entityHandler, ComponentHandler& componentHandler, const int signature, Resources::ResourcesManager resources, std::string name = std::string("No Name") )
			{
				assert(entityHandler.livingEntities < MAX_ENTITIES && "Too many entities in existence.");

				entityHandler.entities[entityHandler.livingEntities].name = name;
				unsigned int id = entityHandler.entities[entityHandler.livingEntities].id;
				entityHandler.livingEntities++;

				if (CheckSignature(signature, SIGNATURE_TRANSFORM))
					componentHandler.AddComponentTransform(entityHandler.entities[id]);
				if (CheckSignature(signature, SIGNATURE_RIGIDBODY))
					componentHandler.AddComponentRigidBody(entityHandler.entities[id]);
				if (CheckSignature(signature, SIGNATURE_MODEL))
					componentHandler.AddComponentModel(entityHandler.entities[id]);

				//not clean should be moved somewhere else
				componentHandler.GetComponentModel(id).shader = resources.GetDefaultShader();
			}
			void RemoveEntity(Entity& entity)
			{
				assert(entityHandler->livingEntities > 0 && "No Entity to remove");

				//Reset Components
				if (CheckSignature(entity.signature, SIGNATURE_TRANSFORM))
					componentHandler->GetComponentTransform(entity.id).ToDefault();
				if (CheckSignature(entity.signature, SIGNATURE_RIGIDBODY))
					componentHandler->GetComponentRigidBody(entity.id).ToDefault();
				if (CheckSignature(entity.signature, SIGNATURE_MODEL))
					componentHandler->GetComponentModel(entity.id).ToDefault();

				//Reset Entity
				entity.signature = 0;
				entity.name = "No Name";
				entity.needToBeRemoved = false;

				//for (unsigned int i = 0; i < entity.children.size(); ++i)
				//	componentHandler->GetComponentTransform(entity.children[i]).parentTRS = Core::Math::Mat4::Identity();

				//Switch the removed one with the last alive
				entityHandler->livingEntities--;
				entity.Swap(entityHandler->entities[entityHandler->livingEntities]);
			}

			static bool CheckSignature(const int entitySignature, const int signature) { return (entitySignature & signature) == signature;	}
			void ApplySystemDisplayId()
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					std::cout << entityHandler->entities[i].id << std::endl;
			}
			void ApplySystemGravity()
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_RIGIDBODY))
						System::SystemGravity(componentHandler->GetComponentRigidBody(entityHandler->entities[i].id));
			}
			void ApplySystemVelocity() 
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_RIGIDBODY))
						System::SystemVelocity(componentHandler->GetComponentTransform(entityHandler->entities[i].id),
											   componentHandler->GetComponentRigidBody(entityHandler->entities[i].id));
			}
			void ApplyDraw(Render::RendererRemote& remote, const Core::Math::Mat4& viewProj)
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_MODEL))
						System::SystemDraw(componentHandler->GetComponentTransform(entityHandler->entities[i].id),
							componentHandler->GetComponentModel(entityHandler->entities[i].id),remote, viewProj);

			}

			void SelectClosestMovableEntity(const Core::Math::Vec3& position)
			{
				float distance = FLT_MAX;

				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_RIGIDBODY))
					{
						float newDistance = (position - componentHandler->GetComponentTransform(entityHandler->entities[i].id).localTRS.translation).Length();
						if (newDistance < distance)
						{
							distance = newDistance;
							selectedEntity = &entityHandler->entities[i];
						}
					}
			}
		};

	}
}

#endif