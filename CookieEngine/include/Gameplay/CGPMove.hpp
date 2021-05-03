#ifndef _CGP_MOVE_HPP__
#define _CGP_MOVE_HPP__

#include "Core/Primitives.hpp"
#include "Render/DebugRenderer.hpp"

#include "Resources/Map.hpp"
#include "Core/Time.hpp"
#include <vector>

namespace Cookie
{
	namespace Gameplay
	{
		enum CGPMOVE_STATE
		{
			E_STATIC,
			E_MOVING,
			E_PUSHED,
			E_WAITING
		};

		//use constexpr, for now it bug
		#define CPGMOVE_CD_BEFORE_RETURN 0.5f


		class CGPMove
		{
		public:
			CGPMOVE_STATE state = CGPMOVE_STATE::E_STATIC;
			float moveSpeed = 5;
			bool  isFlying = false;

			//use it for collision Detection making a circle with trs.pos
			// sqrt(0.5^2 + 0.5^2)
			float radius = 0.7;

			//temporary
			Resources::Tile* lastTile = nullptr;

			//maybe use a vector of Tile*
			std::vector<Core::Math::Vec3> waypoints;

			Core::Math::Vec3* commanderPos = nullptr;
			CGPMove*		  commanderCGPMove = nullptr;
			Core::Math::Vec3  offsetFromCommander;

			float pushedCooldownBeforeReturn = CPGMOVE_CD_BEFORE_RETURN;
			Core::Math::Vec3 posBeforePushed;

			CGPMove() {}
			~CGPMove() {}

			void UpdatePushedCooldown(Resources::Map& map, ECS::ComponentTransform& trs)
			{
				if (state != CGPMOVE_STATE::E_PUSHED)
					return;

				pushedCooldownBeforeReturn -= Core::DeltaTime();
				std::cout << "pushedCooldown Reducing " << pushedCooldownBeforeReturn << "\n";

				if (pushedCooldownBeforeReturn < 0 && map.ApplyPathfinding(map.GetTile(trs.pos), map.GetTile(posBeforePushed)))
				{
					std::cout << "pushedCooldown FINISH\n";
					pushedCooldownBeforeReturn = CPGMOVE_CD_BEFORE_RETURN;
					SetPath(map.GetTile(posBeforePushed), trs);
				}
			}

			void SetPath(Resources::Tile& lastWaypoint, ECS::ComponentTransform& trs)
			{
				waypoints.clear();
				state = CGPMOVE_STATE::E_MOVING;
				commanderPos = nullptr;
				commanderCGPMove = nullptr;
				lastTile = &lastWaypoint;
				Resources::Tile* currentTile = &lastWaypoint;

				//Whatever happen always had the lastWaypoint
				waypoints.emplace(waypoints.begin(), Core::Math::Vec3{ currentTile->pos.x, 0, currentTile->pos.y });

				//if Entity is Flying go as a straight line
				//or if the lastWaypoint has no parent it mean we go to the Tile we are already on so we don't need to continue
				if (isFlying || !currentTile->parent)
					return;

				currentTile = currentTile->parent;
				//if the Tile has no parent it mean it is the start Tile of the path so we don't add it 
				while (currentTile->parent != nullptr)
				{
					Core::Math::Vec3 currentWaypoint = { currentTile->pos.x, 0, currentTile->pos.y };
					Core::Math::Vec3 parentWaypoint = { currentTile->parent->pos.x, 0, currentTile->parent->pos.y };

					//if the direction from currentWaypoint to next Waypoint is the same as the direction from parentWaypoint
					//to currentWaypoin then it's a straigth line so we don't add the waypoint
					if ((waypoints[0] - currentWaypoint).Normalize() != (currentWaypoint - parentWaypoint).Normalize() )
						waypoints.emplace(waypoints.begin(), currentWaypoint);

					currentTile = currentTile->parent;
				}
				
			}
			void SetCommander(CGPMove& _commanderCGPMove, ECS::ComponentTransform& commanderTrs, ECS::ComponentTransform& selfTrs)
			{
				waypoints.clear();
				state = CGPMOVE_STATE::E_MOVING;
				commanderPos = &(commanderTrs.pos);
				commanderCGPMove = &(_commanderCGPMove);
				offsetFromCommander = selfTrs.pos - commanderTrs.pos;
			}
			
			void MoveTowardWaypoint(ECS::ComponentTransform& trs)
			{
				while (waypoints.size() != 0 && (waypoints[0] - trs.pos).Length() < 0.1)
				{
					waypoints.erase(waypoints.begin());

					//check if reach goal here to avoid to set the state each frame
					if (waypoints.size() == 0)
						state = CGPMOVE_STATE::E_STATIC;
				}

				if (waypoints.size() == 0)
					return;


				Core::Math::Vec3 direction = (waypoints[0] - trs.pos).Normalize();
				trs.pos += direction * (moveSpeed * Core::DeltaTime());
				trs.ComputeTRS();
			}
			void MoveWithCommander(ECS::ComponentTransform& trs)
			{
				if (commanderPos == nullptr)
					return;

				Core::Math::Vec3 previousPos = trs.pos;
				trs.pos = *commanderPos + offsetFromCommander;
				trs.ComputeTRS();

				//check if commander is Static
				if (previousPos == trs.pos)
				{
					commanderPos = nullptr;
					commanderCGPMove = nullptr;
					state = CGPMOVE_STATE::E_STATIC;
				}


			}
			
			void PositionPrediction()
			{
				
			}
			void ResolveColision(ECS::ComponentTransform& trsSelf, CGPMove& other, ECS::ComponentTransform& trsOther)
			{
				//Priority High
				if ((state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_STATIC) ||
					(state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_PUSHED) ||
					(state == CGPMOVE_STATE::E_PUSHED && other.state == CGPMOVE_STATE::E_STATIC))
				{
					if (other.state != CGPMOVE_STATE::E_PUSHED)
					{
						other.state = CGPMOVE_STATE::E_PUSHED;
						other.posBeforePushed = trsOther.pos;
					}
					other.pushedCooldownBeforeReturn = CPGMOVE_CD_BEFORE_RETURN;

					Core::Math::Vec3 direction = (trsOther.pos - trsSelf.pos).Normalize();
					trsOther.pos = trsSelf.pos + direction * (radius + other.radius);
					trsOther.ComputeTRS();
				}
				//Priority Medium need some fixes
				else if (state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_MOVING)
				{
					float overlapLength = (radius + other.radius) - (trsSelf.pos - trsOther.pos).Length();

					Core::Math::Vec3 directionSelfToOther = (trsOther.pos - trsSelf.pos).Normalize();
					Core::Math::Vec3 directionSelf = (commanderCGPMove ? commanderCGPMove->waypoints[0] - *commanderPos : waypoints[0] - trsSelf.pos).Normalize();
					Core::Math::Vec3 directionOther = (other.commanderCGPMove ? other.commanderCGPMove->waypoints[0] - *other.commanderPos : other.waypoints[0] - trsOther.pos).Normalize();

					if (directionSelfToOther.Dot(directionSelf) > 0.9) // if they face each other
					{
						trsSelf.pos += Core::Math::Vec3{directionSelf.z, directionSelf.y, -directionSelf.x} * (overlapLength / 2);
						trsOther.pos += Core::Math::Vec3{ directionOther.z, directionOther.y, -directionOther.x } * (overlapLength / 2);

					}
					else // they colidde side by side
					{
						trsSelf.pos += -directionSelfToOther * (overlapLength / 2);
						trsOther.pos += directionSelfToOther * (overlapLength / 2);
					}
					
					if(commanderCGPMove)
						offsetFromCommander = trsSelf.pos - *commanderPos;
					if (other.commanderCGPMove)
						other.offsetFromCommander = trsOther.pos - *other.commanderPos;

					trsSelf.ComputeTRS();
					trsOther.ComputeTRS();
					
				}
				//Priority Low
				else if ((state == CGPMOVE_STATE::E_STATIC && other.state == CGPMOVE_STATE::E_MOVING) ||
						 (state == CGPMOVE_STATE::E_PUSHED && other.state == CGPMOVE_STATE::E_MOVING) ||
					     (state == CGPMOVE_STATE::E_STATIC && other.state == CGPMOVE_STATE::E_PUSHED))
				{
					if (state != CGPMOVE_STATE::E_PUSHED)
					{
						state = CGPMOVE_STATE::E_PUSHED;
						posBeforePushed = trsSelf.pos;
					}
					pushedCooldownBeforeReturn = CPGMOVE_CD_BEFORE_RETURN;

					Core::Math::Vec3 direction = (trsSelf.pos - trsOther.pos).Normalize();
					trsSelf.pos = trsOther.pos + direction * (radius + other.radius);
					trsSelf.ComputeTRS();
				}


			}
			
			void DrawPath(Render::DebugRenderer& debug, ECS::ComponentTransform& trs)
			{
				if(waypoints.size() != 0)
					debug.AddDebugElement(Core::Primitives::CreateLine({ trs.pos.x, 1, trs.pos.z }, { waypoints[0].x, 1, waypoints[0].z }, 0x00FFFF, 0x00FFFF));

				for (int i = 1; i < waypoints.size(); ++i)
				{
					//use 1 for Y so the debug will not be mix up with the map
					debug.AddDebugElement(Core::Primitives::CreateLine({waypoints[i - 1].x, 1, waypoints[i - 1].z}, {waypoints[i].x, 1, waypoints[i].z}, 0x00FFFF, 0x00FFFF));
				}
			}
		};


	}
}

#endif // !_CGP_MOVE_HPP__