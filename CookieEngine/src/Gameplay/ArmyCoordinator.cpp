#include "Gameplay/ArmyCoordinator.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Resources/Prefab.hpp"
#include "ArmyHandler.hpp"


using namespace Cookie::Gameplay;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;


void ArmyCoordinator::Analysis()
{
	//Economic
	if(army->workers.size() + nbOfWorkerInProduction < behavior.stepGoals.nbOfWorker)
		goals.push_back(E_GOALS::E_DEVELOP_WORKER);
	if(behavior.stepGoals.listOfBuildings.size() > 0)
		goals.push_back(E_GOALS::E_DEVELOP_BASE);
	if (army->units.size() + nbOfUnitInProduction < behavior.stepGoals.nbOfUnits)
		goals.push_back(E_GOALS::E_DEVELOP_ARMY);

	//Military
	//if base under attack
	//goals.push_back(E_GOALS::E_DEFEND);
	if (army->workers.size() >= behavior.stepGoals.nbOfWorker &&
		behavior.stepGoals.listOfBuildings.empty() &&
		army->units.size() >= behavior.stepGoals.nbOfUnits)
	{
		goals.push_back(E_GOALS::E_ATTACK);
		behavior.AddNextStep();
	}

}

void ArmyCoordinator::ResourceAllocation()
{
	for (int i = 0; i < goals.size(); ++i)
	{
		switch (goals[i])
		{
		case E_GOALS::E_DEVELOP_WORKER:
			DevelopWorker();
			break;
		case E_GOALS::E_DEVELOP_BASE:
			DevelopBase();
			break;

		case E_GOALS::E_DEVELOP_ARMY:
			DevelopArmy();
			break;

		case E_GOALS::E_DEFENSE:
			Defense();
			break;

		case E_GOALS::E_ATTACK:
			Attack();
			break;

		case E_GOALS::E_RETREAT:
			Retreat();
			break;

		default:
			break;
		}
	}

	goals.clear();
}

void ArmyCoordinator::DevelopWorker()
{
	std::cout << "AI DevelopWorker\n";

	for (int i = 0; i < army->buildings.size(); ++i)
	{
		CGPProducer& producer = army->buildings[i]->componentProducer;

		for (int j = 0; j < producer.possibleUnits.size(); ++j)
		{
			if (producer.possibleUnits[j]->gameplay.signatureGameplay & CGP_SIGNATURE::WORKER &&
				producer.AddUnitToQueue(j))
			{
				nbOfWorkerInProduction++;
				return;
			}
		}
	}

}
void ArmyCoordinator::DevelopBase()
{
	std::cout << "AI DevelopBase\n";

	for (int i = 0; i < army->workers.size(); ++i)
	{
		CGPWorker& worker = army->workers[i]->componentWorker;

		if(!worker.BuildingInConstruction)
			for (int j = 0; j < worker.possibleBuildings.size(); ++j)
			{
				std::string name = worker.possibleBuildings[j]->name;

				for (int k = 0; k < behavior.stepGoals.listOfBuildings.size(); ++k)
					if (name == behavior.stepGoals.listOfBuildings[k])
					{
						Vec3 pos {-40 + 10 * (float)(army->buildings.size() + nbOfBuildingInProduction), 0, -40};
						worker.StartBuilding(pos, j);
						behavior.stepGoals.listOfBuildings.erase(behavior.stepGoals.listOfBuildings.begin() + k);

						nbOfBuildingInProduction++;
						return;
					}

			}
	}
}
void ArmyCoordinator::DevelopArmy()
{
	std::cout << "AI DevelopArmy\n";

	for (int i = 0; i < army->buildings.size(); ++i)
	{
		CGPProducer& producer = army->buildings[i]->componentProducer;

		for (int j = 0; j < producer.possibleUnits.size(); ++j)
		{
			if (producer.possibleUnits[j]->gameplay.signatureGameplay & CGP_SIGNATURE::ATTACK &&
				producer.AddUnitToQueue(j))
			{
				nbOfUnitInProduction++;
				return;
			}
		}
	}

}
void ArmyCoordinator::Attack()
{
	std::cout << "AI Attack\n";
}
void ArmyCoordinator::Defense()
{
	std::cout << "AI Defend\n";
}
void ArmyCoordinator::Retreat()
{
	std::cout << "AI Retreat\n";
}