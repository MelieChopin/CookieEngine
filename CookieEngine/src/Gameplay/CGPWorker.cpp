#include "Gameplay/CGPWorker.hpp"
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"
#include "Gameplay/ArmyHandler.hpp"
#include "ECS/Coordinator.hpp"
#include "ECS/ComponentHandler.hpp"


using namespace Cookie::Gameplay;
using namespace Cookie::ECS;


void CGPWorker::Update(ECS::Coordinator& coordinator, int selfId)
{

	//isBuilding
	if (constructionCountdown)
	{
		constructionCountdown -= Core::DeltaTime();

		if (constructionCountdown <= 0)
			coordinator.AddEntity(BuildingInConstruction);
		

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
	Core::Math::Vec3 destination = (isCarryingResource) ? *posBase : posResource;
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
			std::cout << coordinator.componentHandler->GetComponentGameplay(selfId).teamName << " : " << income->primary << "\n";
		}
		else
			harvestCountdown = TIME_TO_HARVEST;
	}

	
}