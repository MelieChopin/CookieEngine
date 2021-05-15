#ifndef _ARMY_COORDINATOR_HPP__
#define _ARMY_COORDINATOR_HPP__

#include "ArmyHandler.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		enum E_GOAL
		{
			//Economic
			E_DEVELOP_BASE,
			E_DEVELOP_ARMY,
			E_WAIT_ECONOMIC,

			//Military
			E_DEFENSE,
			E_ATTACK,
			E_RETREAT,
			E_WAIT_MILITARY
		};



		class ArmyCoordinator
		{
			Army* army {nullptr};

			ArmyCoordinator() {}
			~ArmyCoordinator() {}

			//void Tactical(); done by the CGPAttack
			void Analysis() {}
			void ResourceAllocation() {}
		};


	}
}

#endif // !_ARMY_COORDINATOR_HPP__