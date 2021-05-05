#ifndef __COORDINATOR_HPP__
#define __COORDINATOR_HPP__

#include "Core/Math/Mat4.hpp"
#include <vector>

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
		class Map;
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
			EntityHandler*     entityHandler    {nullptr};
			ComponentHandler*  componentHandler {nullptr};
			std::vector<Entity*> selectedEntities;

			Coordinator() {};
			~Coordinator() {};

			void AddEntity(const int signature, const Resources::ResourcesManager& resources, std::string name = std::string("No Name"));
			void RemoveEntity(Entity& entity);
			bool CheckSignature(const int entitySignature, const int signature);
			

			void ApplySystemPhysics(float factor);
			void ApplyDraw(const Core::Math::Mat4& viewProj);
			void ApplyScriptUpdate();
			void ApplyRemoveUnnecessaryEntities();


			void ApplyGameplayUpdatePushedCooldown(Resources::Map& map);
			void ApplyGameplayMoveTowardWaypoint();
			void ApplyGameplayMoveWithCommander();
			void ApplyGameplayPosPrediction();
			void ApplyGameplayResolveCollision();
			void ApplyGameplayDrawPath(Render::DebugRenderer& debug);

			void ApplyGameplayCheckEnemyInRange();
			void ApplyGameplayAttack();

			void SelectEntities(Core::Math::Vec2& selectionQuadStart, Core::Math::Vec2& selectionQuadEnd);
			Entity* GetSelectedEntitiesCommander();
			void SetSelectedEntitiesCommander(Entity* commander);
			
		};

	}
}

#endif
