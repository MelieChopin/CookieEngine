#ifndef _CGP_MOVE_HPP__
#define _CGP_MOVE_HPP__

#include "Resources/Map.hpp"
#include "Render/DebugRenderer.hpp"
#include <vector>

namespace Cookie
{
	namespace Gameplay
	{
		enum CGPMOVE_STATE
		{
			E_REACH_GOAL,
			E_STATIC,
			E_WAITING,
			E_PUSHED,
			E_MOVING
		};

		//use constexpr, for now it bug
		#define CGPMOVE_CD_BEFORE_RETURN 0.5f
		#define CGPMOVE_CD_BEFORE_STATIC 2.f
		#define OFFSET_MAX_FROM_CENTROID 5

		class CGPMove
		{
		public:
			CGPMOVE_STATE state = CGPMOVE_STATE::E_STATIC;
			float moveSpeed = 5;
			bool  isFlying = false;
			ECS::ComponentTransform* trs {nullptr};

			//use it for collision Detection making a circle with trs.pos
			// sqrt(scale.x^2 + scale.z^2)
			float radius = 0.7;

			std::vector<Core::Math::Vec3> waypoints;

			float reachGoalCountdown = CGPMOVE_CD_BEFORE_STATIC;
			float pushedCooldownBeforeReturn = CGPMOVE_CD_BEFORE_RETURN;
			Core::Math::Vec3 posBeforePushed;

			CGPMove() {}
			~CGPMove() {}

			inline void ToDefault() noexcept
			{
				state = CGPMOVE_STATE::E_STATIC;
				moveSpeed = 0;
				isFlying = false;
				radius = 0;
				waypoints.clear();
				pushedCooldownBeforeReturn = CGPMOVE_CD_BEFORE_RETURN;
				reachGoalCountdown = CGPMOVE_CD_BEFORE_STATIC;
			}

			void UpdatePushedCooldown(Resources::Map& map);
			void UpdateReachGoalCooldown();

			void SetPath(Resources::Tile& lastWaypoint);
			
			void MoveTowardWaypoint();
			
			void PositionPrediction();
			void ResolveColision(CGPMove& other, Resources::Map& map);
			
			void DrawPath(Render::DebugRenderer& debug);
		};


	}
}

#endif // !_CGP_MOVE_HPP__