#include "ResourcesManager.hpp"
#include "Resources/Prefab.hpp"
#include "Resources/Map.hpp"
#include "Core/Primitives.hpp"
#include "Render/DebugRenderer.hpp"

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentHandler.hpp"
#include "ECS/Coordinator.hpp"

#include "Gameplay/ArmyHandler.hpp"

#include <assert.h>

using namespace Cookie::Resources;
using namespace Cookie::Render;
using namespace Cookie::Gameplay;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;


//Entity
Entity& Coordinator::AddEntity(const int signature, std::string name)
{
	assert(entityHandler->livingEntities < MAX_ENTITIES && "Too many entities in existence.");

	Entity& newEntity = entityHandler->entities[entityHandler->livingEntities];
	entityHandler->livingEntities++;
	newEntity.name = name;

	if (CheckSignature(signature, C_SIGNATURE::TRANSFORM))
		componentHandler->AddComponent(newEntity, C_SIGNATURE::TRANSFORM);
	if (CheckSignature(signature, C_SIGNATURE::MODEL))
		componentHandler->AddComponent(newEntity, C_SIGNATURE::MODEL);
	if (CheckSignature(signature, C_SIGNATURE::PHYSICS))
		componentHandler->AddComponent(newEntity, C_SIGNATURE::PHYSICS);
	if (CheckSignature(signature, C_SIGNATURE::SCRIPT))
		componentHandler->AddComponent(newEntity, C_SIGNATURE::SCRIPT);
	if (CheckSignature(signature, C_SIGNATURE::GAMEPLAY))
		componentHandler->AddComponent(newEntity, C_SIGNATURE::GAMEPLAY);

	return newEntity;
}
Entity& Coordinator::AddEntity(const Resources::Prefab* const & prefab, std::string teamName)
{
	assert(entityHandler->livingEntities < MAX_ENTITIES && "Too many entities in existence." && prefab != nullptr);


	Entity& newEntity = entityHandler->entities[entityHandler->livingEntities];
	entityHandler->livingEntities++;
	newEntity.name = prefab->name;
	newEntity.signature = prefab->signature;

	if (CheckSignature(newEntity.signature, C_SIGNATURE::TRANSFORM))
		componentHandler->GetComponentTransform(newEntity.id) = prefab->transform;
	if (CheckSignature(newEntity.signature, C_SIGNATURE::MODEL))
		componentHandler->GetComponentModel(newEntity.id) = prefab->model;
	if (CheckSignature(newEntity.signature, C_SIGNATURE::PHYSICS))
		componentHandler->GetComponentPhysics(newEntity.id) = prefab->physics;
	//if (CheckSignature(signature, C_SIGNATURE::SCRIPT))
		//componentHandler->GetComponentScript(newEntity.id) = prefab->script;
	if (CheckSignature(newEntity.signature, C_SIGNATURE::GAMEPLAY))
	{
		componentHandler->GetComponentGameplay(newEntity.id) = prefab->gameplay;
		componentHandler->GetComponentGameplay(newEntity.id).teamName = teamName;
		armyHandler->AddElementToArmy(&componentHandler->GetComponentGameplay(newEntity.id));
	}



	return newEntity;
}
void Coordinator::RemoveEntity(Entity& entity)
{
	assert(entityHandler->livingEntities > 0 && "No Entity to remove");

	//Reset Components
	componentHandler->SubComponentToDefault(entity.signature, entity.id);
	//Reset Entity
	entity.ToDefault();

	//Switch the removed one with the last alive
	entityHandler->livingEntities--;
	entity.Swap(entityHandler->entities[entityHandler->livingEntities]);
}
bool Coordinator::CheckSignature(const int entitySignature, const int signature)
{
	return (entitySignature & signature) == signature;
}

//Selection
void Coordinator::SelectEntities(Vec2& selectionQuadStart, Vec2& selectionQuadEnd)
{
	selectedEntities.clear();
	float minX = (selectionQuadStart.x < selectionQuadEnd.x) ? selectionQuadStart.x : selectionQuadEnd.x;
	float maxX = (selectionQuadStart.x < selectionQuadEnd.x) ? selectionQuadEnd.x : selectionQuadStart.x;
	float minZ = (selectionQuadStart.y < selectionQuadEnd.y) ? selectionQuadStart.y : selectionQuadEnd.y;
	float maxZ = (selectionQuadStart.y < selectionQuadEnd.y) ? selectionQuadEnd.y : selectionQuadStart.y;

	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY))
		{
			Vec3& entityPos = componentHandler->GetComponentTransform(entityHandler->entities[i].id).pos;
			if (minX <= entityPos.x && entityPos.x <= maxX &&
				minZ <= entityPos.z && entityPos.z <= maxZ)
				selectedEntities.push_back(&entityHandler->entities[i]);
		}
}
Entity* Coordinator::GetSelectedEntitiesCommander()
{
	float selectedEntitiesSize = selectedEntities.size();

	//get Centroid
	Vec3 centroid = { 0, 0, 0 };
	for (int i = 0; i < selectedEntitiesSize; ++i)
	{
		//divide by selectedEntitiesSize in for loop, so we're sure we can't divide by 0
		centroid += componentHandler->GetComponentTransform(selectedEntities[i]->id).pos / selectedEntitiesSize;
	}

	//get Commander
	Entity* commander = nullptr;
	float distFromCentroid = INFINITY;
	for (int i = 0; i < selectedEntitiesSize; ++i)
	{
		float possibleNewDist = (centroid - componentHandler->GetComponentTransform(selectedEntities[i]->id).pos).Length();
		if (possibleNewDist < distFromCentroid)
		{
			distFromCentroid = possibleNewDist;
			commander = selectedEntities[i];
		}
	}

	return commander;
}
void Coordinator::SetSelectedEntitiesCommander(Entity* commander)
{
	for (int i = 0; i < selectedEntities.size(); ++i)
	{
		if (CheckSignature(selectedEntities[i]->signature, C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(selectedEntities[i]->id).signatureGameplay, CGP_SIGNATURE::MOVE) &&
			selectedEntities[i] != commander)
			componentHandler->GetComponentGameplay(selectedEntities[i]->id).componentMove.SetCommander(componentHandler->GetComponentGameplay(commander->id).componentMove, componentHandler->GetComponentTransform(commander->id), componentHandler->GetComponentTransform(selectedEntities[i]->id));
	}

}



//Primary Component
/*
void Coordinator::ApplySystemPhysics(float factor)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::PHYSICS))
			System::SystemPhysics(componentHandler->GetComponentPhysics(entityHandler->entities[i].id), factor);
}*/
//void Coordinator::ApplyDraw(const Core::Math::Mat4& viewProj, ID3D11Buffer* CBuffer)
//{
//	for (int i = 0; i < entityHandler->livingEntities; ++i)
//		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL))
//			System::SystemDraw(componentHandler->GetComponentTransform(entityHandler->entities[i].id),
//				componentHandler->GetComponentModel(entityHandler->entities[i].id), viewProj, CBuffer);
//}
void Coordinator::ApplyScriptStart()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::SCRIPT))
			componentHandler->GetComponentScript(entityHandler->entities[i].id).Start();
}
void Coordinator::ApplyScriptUpdate()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::SCRIPT))
			componentHandler->GetComponentScript(entityHandler->entities[i].id).Update();
}
void Coordinator::ApplyRemoveUnnecessaryEntities()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (entityHandler->entities[i].needToBeRemoved ||
			(CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::LIVE) && componentHandler->GetComponentGameplay(entityHandler->entities[i].id).componentLive.life <= 0))
		{
			RemoveEntity(entityHandler->entities[i]);
			i = std::max(i - 1, 0);
		}
}
void Coordinator::ApplyComputeTrs()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM))
		{
			ComponentTransform& trs = componentHandler->GetComponentTransform(entityHandler->entities[i].id);
			
			if (trs.trsHasChanged)
			{
				trs.trsHasChanged = false;
				trs.ComputeTRS();
			}
		}
}


//CGP_Producer
void Coordinator::UpdateCGPProducer()
{
	ApplyGameplayUpdateCountdownProducer();
}
void Coordinator::ApplyGameplayUpdateCountdownProducer()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::PRODUCER))
			componentHandler->GetComponentGameplay(i).componentProducer.UpdateCountdown(*this, entityHandler->entities[i].id);
}

//CGP_Worker
void Coordinator::UpdateCGPWorker(Map& map)
{
	ApplyGameplayUpdateWorker(map);
}
void Coordinator::ApplyGameplayUpdateWorker(Map& map)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::WORKER))
			componentHandler->GetComponentGameplay(i).componentWorker.Update(map , *this, entityHandler->entities[i].id);
}

//CGP_Move
void Coordinator::UpdateCGPMove(Resources::Map& map, Render::DebugRenderer& debug)
{
	ApplyGameplayUpdatePushedCooldown(map);
	ApplyGameplayUpdateReachGoalCooldown();

	ApplyGameplayMoveTowardWaypoint();
	ApplyGameplayMoveWithCommander();
	
	ApplyGameplayPosPrediction();
	ApplyGameplayResolveCollision();
	
	ApplyGameplayDrawPath(debug);
}
void Coordinator::ApplyGameplayUpdatePushedCooldown(Resources::Map& map)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.UpdatePushedCooldown(map, componentHandler->GetComponentTransform(i));
}
void Coordinator::ApplyGameplayUpdateReachGoalCooldown()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.UpdateReachGoalCooldown();
}
void Coordinator::ApplyGameplayMoveTowardWaypoint()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.MoveTowardWaypoint(componentHandler->GetComponentTransform(i));
}
void Coordinator::ApplyGameplayMoveWithCommander()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.MoveWithCommander(componentHandler->GetComponentTransform(i));
}
void Coordinator::ApplyGameplayPosPrediction()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.PositionPrediction();
}
void Coordinator::ApplyGameplayResolveCollision()
{
	std::vector<Entity*> entitiesToCheck;

	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
		{

			CGPMove& cgpMoveSelf = componentHandler->GetComponentGameplay(i).componentMove;
			ComponentTransform& trsSelf = componentHandler->GetComponentTransform(i);

			for (int j = 0; j < entitiesToCheck.size(); ++j)
			{
				CGPMove& cgpMoveOther = componentHandler->GetComponentGameplay(entitiesToCheck[j]->id).componentMove;
				ComponentTransform& trsOther = componentHandler->GetComponentTransform(entitiesToCheck[j]->id);

				//if the two circles collide
				if ((trsSelf.pos - trsOther.pos).Length() < cgpMoveSelf.radius + cgpMoveOther.radius)
					cgpMoveSelf.ResolveColision(trsSelf, cgpMoveOther, trsOther);
			}

			entitiesToCheck.push_back(&entityHandler->entities[i]);
		}
	/*
	bool allCollisionResolved = false;
	int counter = 0;

	while (!allCollisionResolved && counter < 20)
	{
		allCollisionResolved = true;
		counter++;

		for (int i = 0; i < entitiesToCheck.size(); ++i)
		{
			CGPMove& cgpMoveSelf = componentHandler->GetComponentGameplay(entitiesToCheck[i]->id).componentMove;
			ComponentTransform& trsSelf = componentHandler->GetComponentTransform(entitiesToCheck[i]->id);

			for (int j = 0; j < entitiesToCheck.size(); ++j)
			{
				if (i != j)
				{
					CGPMove& cgpMoveOther = componentHandler->GetComponentGameplay(entitiesToCheck[j]->id).componentMove;
					ComponentTransform& trsOther = componentHandler->GetComponentTransform(entitiesToCheck[j]->id);

					//if the two circles collide
					if ((trsSelf.pos - trsOther.pos).Length() < cgpMoveSelf.radius + cgpMoveOther.radius - 0.1)
					{
						std::cout << "length : " << (trsSelf.pos - trsOther.pos).Length() << " radius added : " << cgpMoveSelf.radius + cgpMoveOther.radius - 0.1 << "\n";
						allCollisionResolved = false;
						cgpMoveSelf.ResolveColision(trsSelf, cgpMoveOther, trsOther);
					}
				}
			}

		}

	}*/

}
void Coordinator::ApplyGameplayDrawPath(DebugRenderer& debug)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.DrawPath(debug, componentHandler->GetComponentTransform(i));
}

//CGP_Attack
void Coordinator::UpdateCGPAttack()
{
	ApplyGameplayCheckEnemyInRange();
	ApplyGameplayAttack();
}
void Coordinator::ApplyGameplayCheckEnemyInRange()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::ATTACK))
		{
			CGPAttack& cgpAttack = componentHandler->GetComponentGameplay(i).componentAttack;
			cgpAttack.target = nullptr;
			float smallestDist = cgpAttack.attackRange;

			for (int j = 0; j < entityHandler->livingEntities; ++j)
				if (i != j &&
					CheckSignature(entityHandler->entities[j].signature, C_SIGNATURE::TRANSFORM + C_SIGNATURE::GAMEPLAY) &&
					componentHandler->GetComponentGameplay(entityHandler->entities[i].id).teamName != componentHandler->GetComponentGameplay(entityHandler->entities[j].id).teamName &&
					CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[j].id).signatureGameplay, CGP_SIGNATURE::LIVE))
				{
					float possibleNewDist = (componentHandler->GetComponentTransform(i).pos - componentHandler->GetComponentTransform(j).pos).Length();

					if (possibleNewDist < smallestDist)
					{
						smallestDist = possibleNewDist;
						cgpAttack.target = &componentHandler->GetComponentGameplay(j).componentLive;
					}
				}

			if (CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::MOVE))
			{
				CGPMove& cgpMove = componentHandler->GetComponentGameplay(i).componentMove;

				//stop mvt
				if (cgpMove.state == CGPMOVE_STATE::E_MOVING && cgpAttack.target != nullptr)
					cgpMove.state = CGPMOVE_STATE::E_WAITING;

				//resume mvt
				if (cgpMove.state == CGPMOVE_STATE::E_WAITING && cgpAttack.target == nullptr)
					cgpMove.state = CGPMOVE_STATE::E_MOVING;
			}
		}
}
void Coordinator::ApplyGameplayAttack()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, C_SIGNATURE::GAMEPLAY) && 
			CheckSignature(componentHandler->GetComponentGameplay(entityHandler->entities[i].id).signatureGameplay, CGP_SIGNATURE::ATTACK))
			componentHandler->GetComponentGameplay(i).componentAttack.Attack();
}

