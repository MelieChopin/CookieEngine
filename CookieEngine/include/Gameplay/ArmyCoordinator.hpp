#ifndef _ARMY_COORDINATOR_HPP__
#define _ARMY_COORDINATOR_HPP__

#include <vector>

namespace Cookie
{
	namespace Gameplay
	{
		class Army;

		enum E_GOALS
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
		public :

			Army*                army {nullptr};
			std::vector<E_GOALS> goals;

			ArmyCoordinator(Army* _army) : army{_army}  {}
			~ArmyCoordinator() {}


			//void Tactical(); done by the CGPAttack
			void Analysis();
			void ResourceAllocation();

			void DevelopBase();
			void DevelopArmy();
			void Attack();

		};


	}
}

#endif // !_ARMY_COORDINATOR_HPP__