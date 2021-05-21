#ifndef __COORDINATOR_HPP__
#define __COORDINATOR_HPP__

#include "Core/Math/Mat4.hpp"
#include <vector>

namespace Cookie
{
	namespace Gameplay
	{
		class ArmyHandler;
	}

	namespace Resources
	{
		class ResourcesManager;
		class Map;
		class Prefab;
	}
	namespace Render
	{
		class DebugRenderer;
	}


	namespace ECS
	{
		class Entity;
		class EntityHandler;
		class ComponentHandler;

		class Coordinator
		{
		public:
			EntityHandler*         entityHandler    {nullptr};
			ComponentHandler*      componentHandler {nullptr};
			Gameplay::ArmyHandler* armyHandler      {nullptr};
			std::vector<Entity*>   selectedEntities;

			Coordinator() {};
			~Coordinator() {};

			//Entity
			Entity& AddEntity(const int signature, std::string name = std::string("No Name"));
			Entity& AddEntity(const Resources::Prefab* const & prefab, std::string teamName = "No Team");
			void RemoveEntity(Entity& entity);
			bool CheckSignature(const int entitySignature, const int signature);
			
			//Selection
			void SelectEntities(Core::Math::Vec2& selectionQuadStart, Core::Math::Vec2& selectionQuadEnd);
			Entity* GetSelectedEntitiesCommander();
			void SetSelectedEntitiesCommander(Entity* commander);



			//Primary Component
			void ApplySystemPhysics(float factor);
			//void ApplyDraw(const Core::Math::Mat4& viewProj);
			void ApplyScriptUpdate();
			void ApplyRemoveUnnecessaryEntities();
			void ApplyComputeTrs();


			//CGP_Producer
			void UpdateCGPProducer();
			void ApplyGameplayUpdateCountdownProducer();

			//CGP_Worker
			void UpdateCGPWorker();
			void ApplyGameplayUpdateWorker();

			//CGP_Move
			void UpdateCGPMove(Resources::Map& map, Render::DebugRenderer& debug);
			void ApplyGameplayUpdatePushedCooldown(Resources::Map& map);
			void ApplyGameplayUpdateReachGoalCooldown();
			void ApplyGameplayMoveTowardWaypoint();
			void ApplyGameplayMoveWithCommander();
			void ApplyGameplayPosPrediction();
			void ApplyGameplayResolveCollision();
			void ApplyGameplayDrawPath(Render::DebugRenderer& debug);

			//CGP_Attack
			void UpdateCGPAttack();
			void ApplyGameplayCheckEnemyInRange();
			void ApplyGameplayAttack();

		};

	}
}

#endif
