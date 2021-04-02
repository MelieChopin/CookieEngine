#ifndef __COORDINATOR_HPP__
#define __COORDINATOR_HPP__

#include "EntityHandler.hpp"
#include "ComponentHandler.hpp"
#include "SystemHandler.hpp"
#include "ResourcesManager.hpp"
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

			Coordinator() {}
			~Coordinator() {}

			void AddEntity(const int signature, const Resources::ResourcesManager& resources, std::string name = std::string("No Name"))
			{
				assert(entityHandler->livingEntities < MAX_ENTITIES && "Too many entities in existence.");

				entityHandler->entities[entityHandler->livingEntities].name = name;
				unsigned int id = entityHandler->entities[entityHandler->livingEntities].id;
				entityHandler->livingEntities++;

				if (CheckSignature(signature, SIGNATURE_TRANSFORM))
					componentHandler->AddComponentTransform(entityHandler->entities[id]);
				if (CheckSignature(signature, SIGNATURE_MODEL))
					componentHandler->AddComponentModel(entityHandler->entities[id]);
				//if (CheckSignature(signature, SIGNATURE_PHYSICS))
				//	componentHandler->AddComponentPhysics(entityHandler->entities[id], phs);
				if (CheckSignature(signature, SIGNATURE_SCRIPT))
					componentHandler->AddComponentScript(entityHandler->entities[id]);

				//not clean should be moved somewhere else
				componentHandler->GetComponentModel(id).shader = resources.shaders.at("Texture_Shader");
			}
			//will be removed when scene clean
			static void AddEntity(EntityHandler& entityHandler, ComponentHandler& componentHandler, const int signature, const Resources::ResourcesManager& resources, std::string name = std::string("No Name") )
			{
				assert(entityHandler.livingEntities < MAX_ENTITIES && "Too many entities in existence.");

				entityHandler.entities[entityHandler.livingEntities].name = name;
				unsigned int id = entityHandler.entities[entityHandler.livingEntities].id;
				entityHandler.livingEntities++;

				if (CheckSignature(signature, SIGNATURE_TRANSFORM))
					componentHandler.AddComponentTransform(entityHandler.entities[id]);
				if (CheckSignature(signature, SIGNATURE_MODEL))
					componentHandler.AddComponentModel(entityHandler.entities[id]);
				//if (CheckSignature(signature, SIGNATURE_PHYSICS))
				//	componentHandler.AddComponentPhysics(entityHandler.entities[id], phs);
				if (CheckSignature(signature, SIGNATURE_SCRIPT))
					componentHandler.AddComponentScript(entityHandler.entities[id]);

				//not clean should be moved somewhere else
				componentHandler.GetComponentModel(id).shader = resources.shaders.at("Texture_Shader");
			}
			void RemoveEntity(Entity& entity)
			{
				assert(entityHandler->livingEntities > 0 && "No Entity to remove");

				//Reset Components
				if (CheckSignature(entity.signature, SIGNATURE_TRANSFORM))
					componentHandler->GetComponentTransform(entity.id).ToDefault();
				if (CheckSignature(entity.signature, SIGNATURE_MODEL))
					componentHandler->GetComponentModel(entity.id).ToDefault();
				if (CheckSignature(entity.signature, SIGNATURE_PHYSICS))
					componentHandler->GetComponentPhysics(entity.id).ToDefault();
				if (CheckSignature(entity.signature, SIGNATURE_SCRIPT))
					componentHandler->GetComponentScript(entity.id).ToDefault();

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

			void ApplySystemPhysics(float factor) 
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_PHYSICS))
						System::SystemPhysics(componentHandler->GetComponentTransform(entityHandler->entities[i].id),
											   componentHandler->GetComponentPhysics(entityHandler->entities[i].id), factor);
			}

			void ApplyDraw(Render::RendererRemote& remote, const Core::Math::Mat4& viewProj)
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_MODEL))
						System::SystemDraw(componentHandler->GetComponentTransform(entityHandler->entities[i].id),
							componentHandler->GetComponentModel(entityHandler->entities[i].id),remote, viewProj);
			}

			void ApplyScriptUpdate()
			{
				for (int i = 0; i < entityHandler->livingEntities; ++i)
					if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_SCRIPT))
						System::SystemScriptUpdate(componentHandler->GetComponentScript(entityHandler->entities[i].id));
			}
		};

	}
}

#endif
