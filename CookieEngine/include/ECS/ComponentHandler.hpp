#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include <array>

#include "ECS/ComponentTransform.hpp"
#include "ECS/ComponentPhysics.hpp"
#include "ECS/ComponentScript.hpp"
#include "ECS/ComponentModel.hpp"
#include "Debug.hpp"


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


		class ComponentTransform;
		class ComponentModel;
		class ComponentPhysics;
		class ComponentScript;

		class ComponentHandler
		{
		public:

			std::array<ComponentTransform,	MAX_ENTITIES> componentTransforms;

			std::array<ComponentModel,		MAX_ENTITIES> componentModels;

			std::array<ComponentPhysics,	MAX_ENTITIES> componentPhysics;

			std::array<ComponentScript,		MAX_ENTITIES> componentScripts;



			ComponentHandler() {}
			~ComponentHandler() {}

			inline void AddComponentTransform(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_TRANSFORM)
				{
					CDebug.Warning("Component Transform already present");
					return;
				}

				entity.signature += SIGNATURE_TRANSFORM;
			}
			inline void AddComponentModel(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_MODEL)
				{
					CDebug.Warning("Component Model already present");
					return;
				}

				entity.signature += SIGNATURE_MODEL; 
			}
			inline void AddComponentPhysics(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_PHYSICS)
				{
					CDebug.Warning("Component Collider already present");
					return;
				}

				entity.signature += SIGNATURE_PHYSICS;


				InitComponentPhysic(entity);
			}

			void InitComponentPhysic(Entity& entity);

			inline void AddComponentScript(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_SCRIPT)
				{
					CDebug.Warning("Component Script already present");
					return;
				}

				entity.signature += SIGNATURE_SCRIPT;
			}

			void ModifyComponentOfEntityToPrefab(Entity& entity, Cookie::Resources::ResourcesManager& resourcesManager, std::string& namePrefab);

			inline void RemoveComponentTransform(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_TRANSFORM)
				{
					GetComponentTransform(entity.id).ToDefault();
					entity.signature -= SIGNATURE_TRANSFORM;
					return;
				}
				
				CDebug.Warning("No Component Transform present");
			}
			inline void RemoveComponentModel(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_MODEL)
				{
					GetComponentModel(entity.id).ToDefault();
					entity.signature -= SIGNATURE_MODEL;
					return;
				}

				CDebug.Warning("No Component Model present");
			}
			inline void RemoveComponentPhysics(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_PHYSICS)
				{
					GetComponentPhysics(entity.id).ToDefault();
					entity.signature -= SIGNATURE_PHYSICS;
					return;
				}

				CDebug.Warning("No Component RigidBody present");
			}
			inline void RemoveComponentScript(Entity& entity)noexcept
			{
				if (entity.signature & SIGNATURE_SCRIPT)
				{
					GetComponentScript(entity.id).ToDefault();
					entity.signature -= SIGNATURE_SCRIPT;
					return;
				}

				CDebug.Warning("No Component Script present");
			}

			inline ComponentTransform&	GetComponentTransform	(const unsigned int id)noexcept { return componentTransforms[id];	}
			inline ComponentModel&		GetComponentModel		(const unsigned int id)noexcept { return componentModels[id];		}
			inline ComponentPhysics&	GetComponentPhysics		(const unsigned int id)noexcept { return componentPhysics[id];		}
			inline ComponentScript&		GetComponentScript		(const unsigned int id)noexcept { return componentScripts[id];		}
		};

	}
}

#endif
