#include "Gameplay/ArmyCoordinator.hpp"
#include "ArmyHandler.hpp"


using namespace Cookie::Gameplay;


void ArmyCoordinator::Analysis()
{
	//Economic
	goals.push_back((army->workers.size() < 6) ? E_GOALS::E_DEVELOP_BASE : E_GOALS::E_DEVELOP_ARMY);

	//Military
	goals.push_back((army->units.size() < 5) ? E_GOALS::E_WAIT_MILITARY : E_GOALS::E_ATTACK);

}

void ArmyCoordinator::ResourceAllocation()
{
	for (int i = 0; i < goals.size(); ++i)
	{
		switch(goals[i])
		{
		case E_GOALS::E_DEVELOP_BASE :
			DevelopBase();
			break;

		case E_GOALS::E_DEVELOP_ARMY :
			DevelopArmy();
			break;

		case E_GOALS::E_ATTACK :
			Attack();
			break;

		case E_GOALS::E_WAIT_MILITARY:
			//do nothing
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
}
void ArmyCoordinator::DevelopArmy()
{
	std::cout << "AI DevelopArmy\n";
}
void ArmyCoordinator::Attack()
{
	std::cout << "AI Attack\n";
}