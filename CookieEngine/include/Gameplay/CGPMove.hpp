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
			E_MOVING,
			E_PUSHED,
			E_STATIC,
			E_REACH_GOAL,

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

			inline void ToDefault() noexcept
			{
				state = CGPMOVE_STATE::E_STATIC;
				moveSpeed = 0;
				isFlying = false;
				radius = 0;
				lastTile = nullptr;
				waypoints.clear();
				commanderPos = nullptr;
				commanderCGPMove = nullptr;
				offsetFromCommander = { 0, 0, 0 };
				pushedCooldownBeforeReturn = CPGMOVE_CD_BEFORE_RETURN;
			}

			void UpdatePushedCooldown(Resources::Map& map, ECS::ComponentTransform& trs);

			void SetPath(Resources::Tile& lastWaypoint, ECS::ComponentTransform& trs);
			void SetCommander(CGPMove& _commanderCGPMove, ECS::ComponentTransform& commanderTrs, ECS::ComponentTransform& selfTrs);
			
			void MoveTowardWaypoint(ECS::ComponentTransform& trs);
			void MoveWithCommander(ECS::ComponentTransform& trs);
			
			void PositionPrediction();
			void ResolveColision(ECS::ComponentTransform& trsSelf, CGPMove& other, ECS::ComponentTransform& trsOther);
			
			void DrawPath(Render::DebugRenderer& debug, ECS::ComponentTransform& trs);
		};


	}
}

#endif // !_CGP_MOVE_HPP__