#include "Gameplay/CGPProducer.hpp"
#include "Gameplay/ArmyHandler.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"

using namespace Cookie::Gameplay;
using namespace Cookie::ECS;

void CGPProducer::UpdateCountdown(Coordinator& coordinator)
{
	if (queueOfUnits.empty())
		return;

	currentCountdown -= Core::DeltaTime();

	if (currentCountdown <= 0)
	{
		//add to ECS and ArmyHandler
		Entity& newEntity = coordinator.AddEntity(queueOfUnits[0]);
		coordinator.armyHandler->AddElementToArmy(&coordinator.componentHandler->GetComponentGameplay(newEntity.id) );

		queueOfUnits.erase(queueOfUnits.begin());

		//set to next cooldown if ther is still units
		if (!queueOfUnits.empty())
			currentCountdown = queueOfUnits[0]->gameplay.cost.timeToProduce;
	}
	
}

bool CGPProducer::AddUnitToQueue(int indexInPossible)
{
	//should be impossible when UI implemented
	if (indexInPossible > possibleUnits.size())
		return false;
	Resources::Prefab* const & unitToAdd = possibleUnits[indexInPossible];

	//later on add Debug.Log depending on what is blocking the process to give player Feedback
	if (queueOfUnits.size() == CGP_PRODUCER_MAX_IN_QUEUE ||
		unitToAdd->gameplay.cost.costPrimary > income->primary ||
		unitToAdd->gameplay.cost.costSecondary > income->secondary ||
		unitToAdd->gameplay.cost.costSupply + income->supplyCurrent > income->supplyMax)
	{
		std::cout << "not enough Resources to Produce\n";
		return false;
	}

	income->primary -= unitToAdd->gameplay.cost.costPrimary;
	income->secondary -= unitToAdd->gameplay.cost.costSecondary;
	income->supplyCurrent += unitToAdd->gameplay.cost.costSupply;
	queueOfUnits.push_back(unitToAdd);
	return true;
}
void CGPProducer::RemoveUnitFromQueue(int indexInQueue)
{
	//should be impossible when UI implemented
	if (indexInQueue > queueOfUnits.size())
		return;

	income->primary   += queueOfUnits[indexInQueue]->gameplay.cost.costPrimary;
	income->secondary += queueOfUnits[indexInQueue]->gameplay.cost.costSecondary;
	income->supplyCurrent -= queueOfUnits[indexInQueue]->gameplay.cost.costSupply;
	queueOfUnits.erase(queueOfUnits.begin() + indexInQueue);

	//if we removed the first and there is still a queue, start new cooldown
	if (indexInQueue == 0 && !queueOfUnits.empty())
		currentCountdown = queueOfUnits[0]->gameplay.cost.timeToProduce;
}
