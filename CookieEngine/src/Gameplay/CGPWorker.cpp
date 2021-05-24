#include "Gameplay/CGPWorker.hpp"
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"
#include "Resources/Map.hpp"
#include "Gameplay/ArmyHandler.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"


using namespace Cookie::Core::Math;
using namespace Cookie::Gameplay;
using namespace Cookie::ECS;


void CGPWorker::Update(Resources::Map& map, Coordinator& coordinator, int selfId)
{

	//isBuilding
	if (constructionCountdown > 0)
	{
		std::cout << "Worker updating " << constructionCountdown << "\n";
		constructionCountdown -= Core::DeltaTime();


		if (constructionCountdown <= 0)
		{
			if (BuildingInConstruction)
			{
				Entity& newEntity = coordinator.AddEntity(BuildingInConstruction, coordinator.componentHandler->GetComponentGameplay(selfId).teamName);

				ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(newEntity.id);
				CGPProducer& producer   = coordinator.componentHandler->GetComponentGameplay(newEntity.id).componentProducer;

				trs.pos = posBuilding;
				Vec3 posTopLeft = trs.pos - trs.scale / 2;
				map.GiveTilesToBuilding(map.GetTileIndex(posTopLeft), producer);
			}

			BuildingInConstruction = nullptr;
		}
		else
			return;
	}

	//isHarvesting
	if (harvestCountdown > 0)
	{
		harvestCountdown -= Core::DeltaTime();
		if (harvestCountdown <= 0)
			isCarryingResource = true;

		return;
	}

	//isMoving
	ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(selfId);
	Core::Math::Vec3 destination = (needTostartBuilding) ? posBuilding : (isCarryingResource) ? *posBase : posResource;
	Core::Math::Vec3 direction = (destination - trs.pos).Normalize();
	trs.pos += direction * (moveSpeed * Core::DeltaTime());
	trs.trsHasChanged = true;

	//HasReachDestination
	if ((destination - trs.pos).Length() < 0.1)
	{
		if (needTostartBuilding)
		{
			needTostartBuilding = false;
			constructionCountdown = BuildingInConstruction->gameplay.cost.timeToProduce;
		}
		else if (isCarryingResource)
		{
			isCarryingResource = false;
			(isResourcePrimary ? income->primary : income->secondary) += (isResourcePrimary ? PRIMARY_PER_RECOLT : SECONDARY_PER_RECOLT);
			std::cout << coordinator.componentHandler->GetComponentGameplay(selfId).teamName << " : " << income->primary << "\n";
		}
		else
			harvestCountdown = TIME_TO_HARVEST;
	}

	
}

void CGPWorker::StartBuilding(Vec3& _posBuilding, int indexInPossibleBuildings)
{
	//should be impossible when UI implemented
	assert(indexInPossibleBuildings < possibleBuildings.size());

	Resources::Prefab* const& buildingToAdd = possibleBuildings[indexInPossibleBuildings];

	//later on add Debug.Log depending on what is blocking the process to give player Feedback
	if (buildingToAdd->gameplay.cost.costPrimary > income->primary ||
		buildingToAdd->gameplay.cost.costSecondary > income->secondary)
	{
		std::cout << "not enough Resources to Build\n";
		return;
	}

	income->primary   -= buildingToAdd->gameplay.cost.costPrimary;
	income->secondary -= buildingToAdd->gameplay.cost.costSecondary;

	posBuilding = _posBuilding;
	BuildingInConstruction = possibleBuildings[indexInPossibleBuildings];
	needTostartBuilding = true;
}