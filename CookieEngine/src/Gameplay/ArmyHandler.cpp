#include "Gameplay/ArmyHandler.hpp"


using namespace Cookie::Gameplay;


void ArmyHandler::Debug()
{
	for (int i = 0; i < livingArmies; ++i)
	{
		std::cout << " name : "            << armies[i].name             << "\n"
			      << " income primary : "  << armies[i].income.primary   << "\n"
			      << " nb of workers : "   << armies[i].workers.size()   << "\n"
			      << " nb of units : "     << armies[i].units.size()     << "\n"
			      << " nb of buildings : " << armies[i].buildings.size() << "\n\n";
	}
}

void ArmyHandler::UpdateArmyCoordinators(Resources::Map& map)
{
	for (int i = 0; i < armiesCoordinator.size(); ++i)
	{
		armiesCoordinator[i].Analysis();
		armiesCoordinator[i].ResourceAllocation(map);
	}
}

Army* ArmyHandler::GetArmy(E_ARMY_NAME armyName)
{
	for (int i = 0; i < livingArmies; ++i)
		if (armies[i].name == armyName)
			return &armies[i];

	return nullptr;
}
ArmyCoordinator* ArmyHandler::GetArmyCoordinator(E_ARMY_NAME armyName)
{
	for (int i = 0; i < armiesCoordinator.size(); ++i)
		if (armiesCoordinator[i].army->name == armyName)
			return &armiesCoordinator[i];

	return nullptr;
}

void ArmyHandler::AddArmyCoordinator(E_ARMY_NAME armyName)
{
	for (int i = 0; i < livingArmies; ++i)
		if (armies[i].name == armyName)
		{
			armiesCoordinator.push_back(ArmyCoordinator{ &armies[i] });
			return;
		}
}

void ArmyHandler::AddElementToArmy(ECS::ComponentGameplay* element)
{
	for (int i = 0; i < livingArmies; ++i)
		if (armies[i].name == element->teamName)
		{
			AddElementToArmy(armies[i], element);
			return;
		}

	assert(livingArmies < MAX_ARMIES && "Too many armies in existence.");

	Army& newArmy = armies[livingArmies];
	newArmy.name = element->teamName;
	AddElementToArmy(newArmy, element);
	livingArmies++;
}
void ArmyHandler::AddElementToArmy(Army& army, ECS::ComponentGameplay* element)
{
	ArmyCoordinator* armyCoordinator = GetArmyCoordinator(army.name);

	switch (element->type)
	{
	case E_ARMY_TYPE::E_WORKER:
		element->componentWorker.income = &(army.income);
		army.workers.push_back(element);
		if (armyCoordinator)
			armyCoordinator->nbOfWorkerInProduction--;
		break;

	case E_ARMY_TYPE::E_UNIT:
		army.units.push_back(element);
		if (armyCoordinator)
			armyCoordinator->nbOfUnitInProduction--;
		break;

	case E_ARMY_TYPE::E_BUILDING:
		element->componentProducer.income = &(army.income);
		army.income.supplyMax += element->componentProducer.supplyGiven;
		army.buildings.push_back(element);
		if (armyCoordinator)
			armyCoordinator->nbOfBuildingInProduction--;
		break;

	default:
		break;
	}
}

void ArmyHandler::RemoveElementFromArmy(ECS::ComponentGameplay* element, std::string entityName)
{
	for (int i = 0; i < livingArmies; ++i)
		if (armies[i].name == element->teamName)
		{
			RemoveElementFromArmy(armies[i], element, entityName);
			return;
		}
}
void ArmyHandler::RemoveElementFromArmy(Army& army, ECS::ComponentGameplay* element, std::string entityName)
{
	ArmyCoordinator* armyCoordinator = GetArmyCoordinator(army.name);

	switch (element->type)
	{
	case E_ARMY_TYPE::E_WORKER:
		RemoveElementFromVector(army.workers, element);
		break;

	case E_ARMY_TYPE::E_UNIT:
		RemoveElementFromVector(army.units, element);
		break;

	case E_ARMY_TYPE::E_BUILDING:
		army.income.supplyMax -= element->componentProducer.supplyGiven;
		if (armyCoordinator)
			armyCoordinator->behavior.stepGoals.listOfBuildings.push_back(entityName);
		RemoveElementFromVector(army.buildings, element);
		break;

	default:
		break;
	}
}
void ArmyHandler::RemoveElementFromVector(std::vector<ECS::ComponentGameplay*>& vector, ECS::ComponentGameplay* element)
{
	for (int i = 0; i < vector.size(); ++i)
		if (vector[i] == element)
		{
			vector.erase(vector.begin() + i);
			return;
		}
}
