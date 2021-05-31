#ifndef __COMPONENT_HANDLER_HPP__
#define __COMPONENT_HANDLER_HPP__

#include <array>

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentTransform.hpp"
#include "ECS/ComponentModel.hpp"
#include "ECS/ComponentPhysics.hpp"
#include "ECS/ComponentScript.hpp"
#include "ECS/ComponentGameplay.hpp"


namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
	}

	namespace ECS
	{
		enum  C_SIGNATURE
		{
			EMPTY_C       = 0b00000,
			TRANSFORM     = 0b00001,
			MODEL         = 0b00010,
			PHYSICS	      = 0b00100,
			SCRIPT        = 0b01000,
			GAMEPLAY      = 0b10000,
			ALL_C         = 0b11111
		};


		class ComponentHandler
		{
		private:

			std::array<ComponentTransform,	MAX_ENTITIES> componentTransforms;
			std::array<ComponentModel,		MAX_ENTITIES> componentModels;
			std::array<ComponentPhysics,	MAX_ENTITIES> componentPhysics;
			std::array<ComponentScript,		MAX_ENTITIES> componentScripts;
			std::array<ComponentGameplay,   MAX_ENTITIES> componentGameplays;


		public:
			ComponentHandler()
			{
				for (int i = 0; i < MAX_ENTITIES; ++i)
					componentGameplays[i].trs = &componentTransforms[i];
			}
			~ComponentHandler() {}

			inline void AddComponent(Entity& entity, int signature) noexcept;
			inline void RemoveComponent(Entity& entity, int signature) noexcept;
			inline void SubComponentToDefault(int signature, int entityId)noexcept;


			void InitComponentPhysic(Entity& entity);


			//template later on
			inline ComponentTransform& GetComponentTransform (const unsigned int id) noexcept;
			inline ComponentModel&     GetComponentModel     (const unsigned int id) noexcept;
			inline ComponentPhysics&   GetComponentPhysics   (const unsigned int id) noexcept;
			inline ComponentScript&    GetComponentScript    (const unsigned int id) noexcept;
			inline ComponentGameplay&  GetComponentGameplay  (const unsigned int id) noexcept;
		};

	}
}

#include "ECS/ComponentHandler.inl"

#endif
