#ifndef _ARMY_COORDINATOR_HPP__
#define _ARMY_COORDINATOR_HPP__

#include <vector>
#include "Gameplay/AIBehavior.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		class Army;

		enum E_GOALS
		{
			//Economic
			E_DEVELOP_WORKER,
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

			AIBehavior			 behavior; //use a Behavior* when it will be loaded in the RM

			int nbOfWorkerInProduction   {0};
			int nbOfBuildingInProduction {0};
			int nbOfUnitInProduction     {0};


			ArmyCoordinator(Army* _army) : army{_army}
			{
				behavior.steps.push_back(AIStep{});
				behavior.steps[0].nbOfWorker = 6;
				behavior.steps[0].nbOfUnits = 5;
				behavior.steps[0].listOfBuildings.push_back("02Producer");

				behavior.steps.push_back(AIStep{});
				behavior.steps[1].nbOfWorker = 0;
				behavior.steps[1].nbOfUnits = 4;
				behavior.steps[1].listOfBuildings.push_back("02Producer");
				behavior.steps[1].listOfBuildings.push_back("02Producer");
			}
			~ArmyCoordinator() {}


			//void Tactical(); done by the CGPAttack
			void Analysis();
			void ResourceAllocation();

		private:
			void DevelopWorker();
			void DevelopBase();
			void DevelopArmy();
			void Attack();
			void Defense();
			void Retreat();

		};


	}
}

#endif // !_ARMY_COORDINATOR_HPP__