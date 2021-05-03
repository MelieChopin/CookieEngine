#include "ResourcesManager.hpp"
#include "Resources/Map.hpp"
#include "Core/Primitives.hpp"
#include "Render/DebugRenderer.hpp"

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentHandler.hpp"
#include "ECS/SystemHandler.hpp"
#include "ECS/Coordinator.hpp"

#include <assert.h>

using namespace Cookie::Resources;
using namespace Cookie::Render;
using namespace Cookie::Gameplay;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;


void Coordinator::AddEntity(const int signature, const Resources::ResourcesManager& resources, std::string name)
{
	assert(entityHandler->livingEntities < MAX_ENTITIES && "Too many entities in existence.");

	entityHandler->entities[entityHandler->livingEntities].name = name;
	unsigned int id = entityHandler->entities[entityHandler->livingEntities].id;
	entityHandler->livingEntities++;

	if (CheckSignature(signature, SIGNATURE_TRANSFORM))
		componentHandler->AddComponentTransform(entityHandler->entities[id]);
	if (CheckSignature(signature, SIGNATURE_MODEL))
		componentHandler->AddComponentModel(entityHandler->entities[id]);
	//if (CheckSignature(signature, SIGNATURE_PHYSICS))
	//	componentHandler->AddComponentPhysics(entityHandler->entities[id], phs);
	if (CheckSignature(signature, SIGNATURE_SCRIPT))
		componentHandler->AddComponentScript(entityHandler->entities[id]);
	if (CheckSignature(signature, SIGNATURE_GAMEPLAY))
		componentHandler->AddComponentGameplay(entityHandler->entities[id]);

	//not clean should be moved somewhere else
	componentHandler->GetComponentModel(id).shader = resources.shaders.at("Texture_Shader");
}
void Coordinator::RemoveEntity(Entity& entity)
{
	assert(entityHandler->livingEntities > 0 && "No Entity to remove");

	//Reset Components
	if (CheckSignature(entity.signature, SIGNATURE_TRANSFORM))
		componentHandler->GetComponentTransform(entity.id).ToDefault();
	if (CheckSignature(entity.signature, SIGNATURE_MODEL))
		componentHandler->GetComponentModel(entity.id).ToDefault();
	if (CheckSignature(entity.signature, SIGNATURE_PHYSICS))
		componentHandler->GetComponentPhysics(entity.id).ToDefault();
	if (CheckSignature(entity.signature, SIGNATURE_SCRIPT))
		componentHandler->GetComponentScript(entity.id).ToDefault();
	if (CheckSignature(entity.signature, SIGNATURE_GAMEPLAY))
		componentHandler->GetComponentGameplay(entity.id).ToDefault(entity);

	//Reset Entity
	entity.signature = 0;
	entity.name = "No Name";
	entity.needToBeRemoved = false;

	//for (unsigned int i = 0; i < entity.children.size(); ++i)
	//	componentHandler->GetComponentTransform(entity.children[i]).parentTRS = Mat4::Identity();

	//Switch the removed one with the last alive
	entityHandler->livingEntities--;
	entity.Swap(entityHandler->entities[entityHandler->livingEntities]);
}
bool Coordinator::CheckSignature(const int entitySignature, const int signature)
{
	return (entitySignature & signature) == signature;
}


void Coordinator::ApplySystemPhysics(float factor)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_PHYSICS))
			System::SystemPhysics(componentHandler->GetComponentPhysics(entityHandler->entities[i].id), factor);
}
void Coordinator::ApplyDraw(const Mat4& viewProj)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_MODEL))
			System::SystemDraw(componentHandler->GetComponentTransform(entityHandler->entities[i].id),
				componentHandler->GetComponentModel(entityHandler->entities[i].id), viewProj);
}
void Coordinator::ApplyScriptUpdate()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_SCRIPT))
			System::SystemScriptUpdate(componentHandler->GetComponentScript(entityHandler->entities[i].id));
}


void Coordinator::ApplyGameplayUpdatePushedCooldown(Resources::Map& map)
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM) &&
			CheckSignature(entityHandler->entities[i].signatureGameplay, SIGNATURE_CGP_MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.UpdatePushedCooldown(map, componentHandler->GetComponentTransform(i));
}
void Coordinator::ApplyGameplayMoveTowardWaypoint()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM) &&
			CheckSignature(entityHandler->entities[i].signatureGameplay, SIGNATURE_CGP_MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.MoveTowardWaypoint(componentHandler->GetComponentTransform(i) );
}
void Coordinator::ApplyGameplayMoveWithCommander()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM) &&
			CheckSignature(entityHandler->entities[i].signatureGameplay, SIGNATURE_CGP_MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.MoveWithCommander(componentHandler->GetComponentTransform(i));
}
void Coordinator::ApplyGameplayPosPrediction()
{
	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM) &&
			CheckSignature(entityHandler->entities[i].signatureGameplay, SIGNATURE_CGP_MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.PositionPrediction();
}
void Coordinator::ApplyGameplayResolveCollision()
{
	std::vector<Entity*> entitiesToCheck;

	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM + SIGNATURE_GAMEPLAY) &&
			CheckSignature(entityHandler->entities[i].signatureGameplay, SIGNATURE_CGP_MOVE))
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
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM) &&
			CheckSignature(entityHandler->entities[i].signatureGameplay, SIGNATURE_CGP_MOVE))
			componentHandler->GetComponentGameplay(i).componentMove.DrawPath(debug, componentHandler->GetComponentTransform(i));
}


void Coordinator::SelectEntities(Vec2& selectionQuadStart, Vec2& selectionQuadEnd)
{
	selectedEntities.clear();
	float minX = (selectionQuadStart.x < selectionQuadEnd.x) ? selectionQuadStart.x : selectionQuadEnd.x;
	float maxX = (selectionQuadStart.x < selectionQuadEnd.x) ? selectionQuadEnd.x : selectionQuadStart.x;
	float minZ = (selectionQuadStart.y < selectionQuadEnd.y) ? selectionQuadStart.y : selectionQuadEnd.y;
	float maxZ = (selectionQuadStart.y < selectionQuadEnd.y) ? selectionQuadEnd.y : selectionQuadStart.y;

	for (int i = 0; i < entityHandler->livingEntities; ++i)
		if (CheckSignature(entityHandler->entities[i].signature, SIGNATURE_TRANSFORM))
		{
			Vec3& entityPos = componentHandler->GetComponentTransform(i).pos;
			if (minX <= entityPos.x && entityPos.x <= maxX && 
				minZ <= entityPos.z && entityPos.z <= maxZ)
				selectedEntities.push_back(&entityHandler->entities[i]);
		}
}
Entity* Coordinator::GetSelectedEntitiesCommander()
{
	float selectedEntitiesSize = selectedEntities.size();

	//get Centroid
	Vec3 centroid = {0, 0, 0};
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
		if (CheckSignature(selectedEntities[i]->signature, SIGNATURE_GAMEPLAY) &&
			CheckSignature(selectedEntities[i]->signatureGameplay, SIGNATURE_CGP_MOVE) &&
			selectedEntities[i] != commander)
			componentHandler->GetComponentGameplay(selectedEntities[i]->id).componentMove.SetCommander(componentHandler->GetComponentGameplay(commander->id).componentMove, componentHandler->GetComponentTransform(commander->id), componentHandler->GetComponentTransform(selectedEntities[i]->id));
	}

}