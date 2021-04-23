#ifndef _CGP_MOVE_HPP__
#define _CGP_MOVE_HPP__

#include "Map.hpp"
#include "Core/Time.hpp"
#include <vector>

namespace Cookie
{
	namespace Gameplay
	{
		class CGPMove
		{
		public:
			float moveSpeed = 10;
			std::vector<Core::Math::Vec3> waypoints;
			bool  isFlying = false;

			CGPMove() {}
			~CGPMove() {}

			void MoveTowardWaypoint(ECS::ComponentTransform& trs)
			{
				while (waypoints.size() != 0 && (waypoints[0] - trs.pos).Length() < 0.1)
					waypoints.erase(waypoints.begin());

				if (waypoints.size() == 0)
					return;

				Core::Math::Vec3 direction = (waypoints[0] - trs.pos).Normalize();
				trs.pos += direction * (moveSpeed * Core::DeltaTime());
				trs.ComputeTRS();
			}
			void SetPath(Resources::Tile& lastWaypoint)
			{
				waypoints.clear();
				Resources::Tile* currentWaypoint = &lastWaypoint;

				if (isFlying)
				{
					waypoints.emplace(waypoints.begin(), Core::Math::Vec3{ currentWaypoint->pos.x, 0, currentWaypoint->pos.y });
					return;
				}

				while (currentWaypoint->parent != nullptr)
				{
					waypoints.emplace(waypoints.begin(), Core::Math::Vec3{ currentWaypoint->pos.x, 0, currentWaypoint->pos.y });
					currentWaypoint = currentWaypoint->parent;
				}

			}
		};


	}
}

#endif // !_CGP_MOVE_HPP__