#ifndef __COMPONENT_GAMEPLAY_HPP__
#define __COMPONENT_GAMEPLAY_HPP__

#include "EntityHandler.hpp"
#include "Gameplay/CGPLive.hpp"
#include "Gameplay/CGPMove.hpp"
#include "Gameplay/CGPAttack.hpp"

namespace Cookie
{
	namespace ECS
	{
		#define SIGNATURE_CGP_EMPTY         0b000
		#define SIGNATURE_CGP_LIVE          0b001
		#define SIGNATURE_CGP_MOVE          0b010
		#define SIGNATURE_CGP_ATTACK		0b100
		#define SIGNATURE_CGP_ALL           0b111

		class ComponentGameplay
		{

		public:
			Gameplay::CGPLive   componentLive;
			Gameplay::CGPMove   componentMove;
			Gameplay::CGPAttack componentAttack;


			inline void AddComponentLive(Entity& entity) noexcept;
			inline void AddComponentMove(Entity& entity) noexcept;
			inline void AddComponentAttack(Entity& entity) noexcept;

			inline void RemoveComponentLive(Entity& entity) noexcept;
			inline void RemoveComponentMove(Entity& entity) noexcept;
			inline void RemoveComponentAttack(Entity& entity) noexcept;
				
			inline void ToDefault(Entity& entity) noexcept;

		};
	}
}

#include "ComponentGameplay.inl"

#endif // !__COMPONENT_GAMEPLAY_HPP__
