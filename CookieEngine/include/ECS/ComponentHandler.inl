#ifndef __COMPONENT_HANDLER_INL__
#define __COMPONENT_HANDLER_INL__

#include "Debug.hpp"

namespace Cookie
{
	namespace ECS
	{

		inline void ComponentHandler::AddComponentTransform(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_TRANSFORM)
			{
				CDebug.Warning("Component Transform already present");
				return;
			}

			entity.signature += SIGNATURE_TRANSFORM;
		}
		inline void ComponentHandler::AddComponentModel(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_MODEL)
			{
				CDebug.Warning("Component Model already present");
				return;
			}

			entity.signature += SIGNATURE_MODEL;
		}
		inline void ComponentHandler::AddComponentPhysics(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_PHYSICS)
			{
				CDebug.Warning("Component Collider already present");
				return;
			}

			entity.signature += SIGNATURE_PHYSICS;


			InitComponentPhysic(entity);
		}
		inline void ComponentHandler::AddComponentScript(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_SCRIPT)
			{
				CDebug.Warning("Component Script already present");
				return;
			}

			entity.signature += SIGNATURE_SCRIPT;
		}
		inline void ComponentHandler::AddComponentGameplay(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_GAMEPLAY)
			{
				CDebug.Warning("Component Gameplay already present");
				return;
			}

			entity.signature += SIGNATURE_GAMEPLAY;
		}

		inline void ComponentHandler::RemoveComponentTransform(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_TRANSFORM)
			{
				GetComponentTransform(entity.id).ToDefault();
				entity.signature -= SIGNATURE_TRANSFORM;
				return;
			}

			CDebug.Warning("No Component Transform present");
		}
		inline void ComponentHandler::RemoveComponentModel(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_MODEL)
			{
				GetComponentModel(entity.id).ToDefault();
				entity.signature -= SIGNATURE_MODEL;
				return;
			}

			CDebug.Warning("No Component Model present");
		}
		inline void ComponentHandler::RemoveComponentPhysics(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_PHYSICS)
			{
				GetComponentPhysics(entity.id).ToDefault();
				entity.signature -= SIGNATURE_PHYSICS;
				return;
			}

			CDebug.Warning("No Component RigidBody present");
		}
		inline void ComponentHandler::RemoveComponentScript(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_SCRIPT)
			{
				GetComponentScript(entity.id).ToDefault();
				entity.signature -= SIGNATURE_SCRIPT;
				return;
			}

			CDebug.Warning("No Component Script present");
		}
		inline void ComponentHandler::RemoveComponentGameplay(Entity& entity)noexcept
		{
			if (entity.signature & SIGNATURE_GAMEPLAY)
			{
				GetComponentGameplay(entity.id).ToDefault(entity);
				entity.signature -= SIGNATURE_GAMEPLAY;
				return;
			}

			CDebug.Warning("No Component Gameplay present");
		}

		inline ComponentTransform& ComponentHandler::GetComponentTransform(const unsigned int id) noexcept 
		{
			return componentTransforms[id];
		}
		inline ComponentModel& ComponentHandler::GetComponentModel(const unsigned int id) noexcept 
		{ 
			return componentModels[id];
		}
		inline ComponentPhysics& ComponentHandler::GetComponentPhysics(const unsigned int id) noexcept
		{
			return componentPhysics[id];
		}
		inline ComponentScript& ComponentHandler::GetComponentScript(const unsigned int id) noexcept
		{
			return componentScripts[id]; 
		}
		inline ComponentGameplay& ComponentHandler::GetComponentGameplay(const unsigned int id) noexcept
		{
			return componentGameplays[id];
		}
	}
}


#endif