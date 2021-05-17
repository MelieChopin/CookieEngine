#ifndef __COMPONENT_GAMEPLAY_HPP__
#define __COMPONENT_GAMEPLAY_HPP__

#include "EntityHandler.hpp"
#include "Gameplay/CGPLive.hpp"
#include "Gameplay/CGPMove.hpp"
#include "Gameplay/CGPAttack.hpp"
#include "Gameplay/CGPProducer.hpp"
#include "Gameplay/CGPWorker.hpp"


namespace Cookie
{
	namespace Gameplay
	{
		enum E_ARMY_TYPE
		{
			E_DEFAULT,
			E_WORKER,
			E_UNIT,
			E_BUILDING
		};
	}

	namespace ECS
	{
		enum CGP_SIGNATURE
		{
			EMPTY_CGP     = 0b00000,
			LIVE          = 0b00001,
			MOVE          = 0b00010,
			ATTACK	      = 0b00100,
			PRODUCER      = 0b01000,
			WORKER        = 0b10000,
			ALL_CGP       = 0b11111
		};

		struct Cost
		{
			float timeToProduce {0};
			float costPrimary   {0};
			float costSecondary {0};
			float costSupply    {0};
		};

		class ComponentGameplay
		{

		public:
			std::string            teamName {"No Team"};
			int                    signatureGameplay{ 0 };

			Gameplay::CGPLive      componentLive;
			Gameplay::CGPMove      componentMove;
			Gameplay::CGPAttack    componentAttack;
			Gameplay::CGPProducer  componentProducer;
			Gameplay::CGPWorker    componentWorker;
			Cost                   cost;
			Gameplay::E_ARMY_TYPE  type {Gameplay::E_ARMY_TYPE::E_DEFAULT};

			inline void AddComponent(int ComponentSignature) noexcept;
			inline void RemoveComponent(int ComponentSignature)noexcept;

			inline void ToDefault() noexcept;
			inline void SubComponentToDefault(int ComponentSignature)noexcept;

		};
	}
}

#include "ComponentGameplay.inl"

#endif // !__COMPONENT_GAMEPLAY_HPP__
