#include "Gameplay/CGPProducer.hpp"
#include "Gameplay/ArmyHandler.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"

using namespace Cookie::Gameplay;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;

void CGPProducer::UpdateCountdown(Resources::Map& map, Coordinator& coordinator, int selfId)
{

	if (queueOfUnits.size() == 0)
		return;

	std::cout << "producer updating " << currentCountdown << "\n";
	currentCountdown -= Core::DeltaTime();

	if (currentCountdown <= 0)
	{
		Entity& newEntity = coordinator.AddEntity(queueOfUnits[0], coordinator.componentHandler->GetComponentGameplay(selfId).teamName);

		ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(selfId);
		ComponentTransform& newEntityTrs = coordinator.componentHandler->GetComponentTransform(newEntity.id);
		newEntityTrs.pos = trs.pos;

		//Need to set additionnal behavior if newEntity has a ComponentGameplay
		if (newEntity.signature & C_SIGNATURE::GAMEPLAY)
		{
			ComponentGameplay& newEntityGameplay = coordinator.componentHandler->GetComponentGameplay(newEntity.id);

			if (newEntityGameplay.signatureGameplay & CGP_SIGNATURE::MOVE)
			{
				for (int i = 0; i < occupiedTiles.size(); ++i)
					occupiedTiles[i]->isObstacle = false;

				if (map.ApplyPathfinding(map.GetTile(newEntityTrs.pos), map.GetTile(newUnitDestination)) )
					newEntityGameplay.componentMove.SetPath(map.GetTile(newUnitDestination), newEntityTrs);

				for (int i = 0; i < occupiedTiles.size(); ++i)
					occupiedTiles[i]->isObstacle = true;
			}

			if (newEntityGameplay.signatureGameplay & CGP_SIGNATURE::WORKER)
			{
				newEntityGameplay.componentWorker.posBase = &trs.pos;

				Entity* resourceEntity = coordinator.GetClosestFreeResourceEntity(trs.pos);
				if (resourceEntity)
					newEntityGameplay.componentWorker.SetResource(coordinator.componentHandler->GetComponentTransform(resourceEntity->id).pos, coordinator.componentHandler->GetComponentGameplay(resourceEntity->id).componentResource);

			}
		}

		queueOfUnits.erase(queueOfUnits.begin());

		//set to next cooldown if ther is still units
		if (!queueOfUnits.empty())
			currentCountdown = queueOfUnits[0]->gameplay.cost.timeToProduce;
	}
	
}

bool CGPProducer::AddUnitToQueue(int indexInPossible)
{
	//should be impossible when UI implemented
	assert(indexInPossible < possibleUnits.size());

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

	std::cout << "Add to queue of Producer\n";


	income->primary -= unitToAdd->gameplay.cost.costPrimary;
	income->secondary -= unitToAdd->gameplay.cost.costSecondary;
	income->supplyCurrent += unitToAdd->gameplay.cost.costSupply;
	if (queueOfUnits.size() == 0)
		currentCountdown = unitToAdd->gameplay.cost.timeToProduce;
	queueOfUnits.push_back(unitToAdd);

	return true;
}
void CGPProducer::RemoveUnitFromQueue(int indexInQueue)
{
	//should be impossible when UI implemented
	assert(indexInQueue < queueOfUnits.size());

	std::cout << "remove from queue of Producer\n";


	income->primary   += queueOfUnits[indexInQueue]->gameplay.cost.costPrimary;
	income->secondary += queueOfUnits[indexInQueue]->gameplay.cost.costSecondary;
	income->supplyCurrent -= queueOfUnits[indexInQueue]->gameplay.cost.costSupply;
	queueOfUnits.erase(queueOfUnits.begin() + indexInQueue);

	//if we removed the first and there is still a queue, start new cooldown
	if (indexInQueue == 0 && !queueOfUnits.empty())
		currentCountdown = queueOfUnits[0]->gameplay.cost.timeToProduce;
}
