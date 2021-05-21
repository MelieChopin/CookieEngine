#include "Gameplay/ArmyCoordinator.hpp"
#include "ECS/ComponentGameplay.hpp"
#include "Resources/Prefab.hpp"
#include "ArmyHandler.hpp"


using namespace Cookie::Gameplay;
using namespace Cookie::ECS;


void ArmyCoordinator::Analysis()
{
	//Economic
	goals.push_back((army->workers.size() + nbOfWorkerInProduction < 6) ? E_GOALS::E_DEVELOP_BASE : E_GOALS::E_DEVELOP_ARMY);

	//Military
	goals.push_back((army->units.size() + nbOfUnitInProduction < 5) ? E_GOALS::E_DEFENSE : E_GOALS::E_ATTACK);

}

void ArmyCoordinator::ResourceAllocation()
{
	for (int i = 0; i < goals.size(); ++i)
	{
		switch (goals[i])
		{
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

void ArmyCoordinator::DevelopBase()
{
	std::cout << "AI DevelopBase\n";

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