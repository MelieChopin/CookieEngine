#ifndef __COMPONENT_GAMEPLAY_INL__
#define __COMPONENT_GAMEPLAY_INL__

#include "Debug.hpp"

namespace Cookie
{
	namespace ECS
	{
		inline void ComponentGameplay::AddComponentLive(Entity& entity)noexcept
		{
			if (entity.signatureGameplay & SIGNATURE_CGP_LIVE)
			{
				CDebug.Warning("Component Gameplay Live already present");
				return;
			}

			entity.signatureGameplay += SIGNATURE_CGP_LIVE;
		}
		inline void ComponentGameplay::AddComponentMove(Entity& entity)noexcept
		{
			if (entity.signatureGameplay & SIGNATURE_CGP_MOVE)
			{
				CDebug.Warning("Component Gameplay Move already present");
				return;
			}

			entity.signatureGameplay += SIGNATURE_CGP_MOVE;
		}
		inline void ComponentGameplay::AddComponentAttack(Entity& entity)noexcept
		{
			if (entity.signatureGameplay & SIGNATURE_CGP_ATTACK)
			{
				CDebug.Warning("Component Gameplay Attack already present");
				return;
			}

			entity.signatureGameplay += SIGNATURE_CGP_ATTACK;
		}

		inline void ComponentGameplay::RemoveComponentLive(Entity& entity)noexcept
		{
			if (entity.signatureGameplay & SIGNATURE_CGP_LIVE)
			{
				entity.signatureGameplay -= SIGNATURE_CGP_LIVE;
				return;
			}

			CDebug.Warning("No Component Gameplay Live present");
		}
		inline void ComponentGameplay::RemoveComponentMove(Entity& entity)noexcept
		{
			if (entity.signatureGameplay & SIGNATURE_CGP_MOVE)
			{
				entity.signatureGameplay -= SIGNATURE_CGP_MOVE;
				return;
			}

			CDebug.Warning("No Component Gameplay Move present");
		}
		inline void ComponentGameplay::RemoveComponentAttack(Entity& entity)noexcept
		{
			if (entity.signatureGameplay & SIGNATURE_CGP_ATTACK)
			{
				entity.signatureGameplay -= SIGNATURE_CGP_ATTACK;
				return;
			}

			CDebug.Warning("No Component Gameplay Attack present");
		}

		inline void ComponentGameplay::ToDefault(Entity& entity)noexcept
		{
			entity.signatureGameplay = SIGNATURE_CGP_EMPTY;
		}

	}
}


#endif