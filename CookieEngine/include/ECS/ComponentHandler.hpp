#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include <array>

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentTransform.hpp"
#include "ECS/ComponentModel.hpp"
#include "ECS/ComponentPhysics.hpp"
#include "ECS/ComponentScript.hpp"


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

			std::array<ComponentTransform,	MAX_ENTITIES> componentTransforms;
			std::array<ComponentModel,		MAX_ENTITIES> componentModels;
			std::array<ComponentPhysics,	MAX_ENTITIES> componentPhysics;
			std::array<ComponentScript,		MAX_ENTITIES> componentScripts;



			ComponentHandler() {}
			~ComponentHandler() {}

			inline void AddComponentTransform (Entity& entity) noexcept;
			inline void AddComponentModel     (Entity& entity) noexcept;
			inline void AddComponentPhysics   (Entity& entity) noexcept;
			inline void AddComponentScript    (Entity& entity) noexcept;

			void InitComponentPhysic(Entity& entity);
			void ModifyComponentOfEntityToPrefab(Entity& entity, Cookie::Resources::ResourcesManager& resourcesManager, std::string& namePrefab);

			inline void RemoveComponentTransform (Entity& entity) noexcept;
			inline void RemoveComponentModel     (Entity& entity) noexcept;
			inline void RemoveComponentPhysics   (Entity& entity) noexcept;
			inline void RemoveComponentScript    (Entity& entity) noexcept;

			inline ComponentTransform& GetComponentTransform (const unsigned int id) noexcept;
			inline ComponentModel&     GetComponentModel     (const unsigned int id) noexcept;
			inline ComponentPhysics&   GetComponentPhysics   (const unsigned int id) noexcept;
			inline ComponentScript&    GetComponentScript    (const unsigned int id) noexcept;
		};

	}
}

#include "ECS/ComponentHandler.inl"

#endif
