#include "Gameplay/CGPWorker.hpp"
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"
#include "Gameplay/ArmyHandler.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"


using namespace Cookie::Gameplay;
using namespace Cookie::ECS;


void CGPWorker::Update(ECS::ComponentTransform& trs, Coordinator& coordinator)
{

	//isBuilding
	if (constructionCountdown)
	{
		constructionCountdown -= Core::DeltaTime();
		if (constructionCountdown <= 0)
		{
			//add to ECS and ArmyHandler
			Entity& newEntity = coordinator.AddEntity(BuildingInConstruction);
			coordinator.armyHandler->AddElementToArmy(&coordinator.componentHandler->GetComponentGameplay(newEntity.id));
		}

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
	Core::Math::Vec3 destination = (isCarryingResource) ? posBase : posResource;
	Core::Math::Vec3 direction = (destination - trs.pos).Normalize();
	trs.pos += direction * (moveSpeed * Core::DeltaTime());
	trs.trsHasChanged = true;

	//HasReachDestination
	if ((destination - trs.pos).Length() < 0.1)
	{
		if (isCarryingResource)
		{
			isCarryingResource = false;
			(isResourcePrimary ? income->primary : income->secondary) += (isResourcePrimary ? PRIMARY_PER_RECOLT : SECONDARY_PER_RECOLT);
			std::cout << income->primary << "\n";
		}
		else
			harvestCountdown = TIME_TO_HARVEST;
	}

	
}