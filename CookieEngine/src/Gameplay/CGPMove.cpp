#include "Gameplay/CGPMove.hpp"
#include "Core/Primitives.hpp"
#include "Core/Time.hpp"

using namespace Cookie::Gameplay;

void CGPMove::UpdatePushedCooldown(Resources::Map& map, ECS::ComponentTransform& trs)
{
	if (state != CGPMOVE_STATE::E_PUSHED)
		return;

	pushedCooldownBeforeReturn -= Core::DeltaTime();

	if (pushedCooldownBeforeReturn < 0 && map.ApplyPathfinding(map.GetTile(trs.pos), map.GetTile(posBeforePushed)))
	{
		//reset of timer should be remove after test
		pushedCooldownBeforeReturn = CGPMOVE_CD_BEFORE_RETURN;
		SetPath(map.GetTile(posBeforePushed), trs);
	}
}
void CGPMove::UpdateReachGoalCooldown()
{
	if (state != CGPMOVE_STATE::E_REACH_GOAL)
		return;

	reachGoalCountdown -= Core::DeltaTime();

	if (reachGoalCountdown <= 0)
	{
		reachGoalCountdown = CGPMOVE_CD_BEFORE_STATIC;
		state = CGPMOVE_STATE::E_STATIC;
	}

}

void CGPMove::SetPath(Resources::Tile& lastWaypoint, ECS::ComponentTransform& trs)
{
	waypoints.clear();
	state = CGPMOVE_STATE::E_MOVING;
	Resources::Tile* currentTile = &lastWaypoint;

	//Whatever happen always had the lastWaypoint
	waypoints.emplace(waypoints.begin(), Core::Math::Vec3{ currentTile->pos.x, 1, currentTile->pos.y });

	//if Entity is Flying go as a straight line
	//or if the lastWaypoint has no parent it mean we go to the Tile we are already on so we don't need to continue
	if (isFlying || !currentTile->parent)
		return;

	currentTile = currentTile->parent;
	//if the Tile has no parent it mean it is the start Tile of the path so we don't add it 
	while (currentTile->parent != nullptr)
	{
		Core::Math::Vec3 currentWaypoint = { currentTile->pos.x, 1, currentTile->pos.y };
		Core::Math::Vec3 parentWaypoint = { currentTile->parent->pos.x, 1, currentTile->parent->pos.y };

		//if the direction from currentWaypoint to next Waypoint is the same as the direction from parentWaypoint
		//to currentWaypoin then it's a straigth line so we don't add the waypoint
		if ((waypoints[0] - currentWaypoint).Normalize() != (currentWaypoint - parentWaypoint).Normalize())
			waypoints.emplace(waypoints.begin(), currentWaypoint);

		currentTile = currentTile->parent;
	}

}

void CGPMove::MoveTowardWaypoint(ECS::ComponentTransform& trs)
{
	while (waypoints.size() != 0 && (waypoints[0] - trs.pos).Length() < 0.1)
	{
		waypoints.erase(waypoints.begin());

		//check if reach goal here to avoid to set the state each frame
		if (waypoints.size() == 0)
			state = CGPMOVE_STATE::E_REACH_GOAL;
	}

	if (waypoints.size() == 0 || state == CGPMOVE_STATE::E_WAITING)
		return;


	Core::Math::Vec3 direction = (waypoints[0] - trs.pos).Normalize();
	trs.pos += direction * (moveSpeed * Core::DeltaTime());
	trs.trsHasChanged = true;
}

void CGPMove::PositionPrediction()
{

}
void CGPMove::ResolveColision(ECS::ComponentTransform& trsSelf, CGPMove& other, ECS::ComponentTransform& trsOther)
{
	//Priority High
	if ((state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_PUSHED) ||
		(state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_STATIC) ||
		(state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_REACH_GOAL) ||
		(state == CGPMOVE_STATE::E_PUSHED && other.state == CGPMOVE_STATE::E_STATIC) ||
		(state == CGPMOVE_STATE::E_PUSHED && other.state == CGPMOVE_STATE::E_REACH_GOAL) ||
		(state == CGPMOVE_STATE::E_REACH_GOAL && other.state == CGPMOVE_STATE::E_REACH_GOAL && reachGoalCountdown >= other.reachGoalCountdown))
	{
		if (other.state == CGPMOVE_STATE::E_STATIC)
		{
			other.state = CGPMOVE_STATE::E_PUSHED;
			other.posBeforePushed = trsOther.pos;
		}
		other.pushedCooldownBeforeReturn = CGPMOVE_CD_BEFORE_RETURN;

		Core::Math::Vec3 direction = (trsOther.pos - trsSelf.pos).Normalize();
		trsOther.pos = trsSelf.pos + direction * (radius + other.radius);
		trsOther.trsHasChanged = true;
	}
	//Priority Medium need some fixes
	else if (state == CGPMOVE_STATE::E_MOVING && other.state == CGPMOVE_STATE::E_MOVING)
	{
		float overlapLength = (radius + other.radius) - (trsSelf.pos - trsOther.pos).Length();

		Core::Math::Vec3 directionSelfToOther = (trsOther.pos - trsSelf.pos).Normalize();
		Core::Math::Vec3 directionSelf = (waypoints[0] - trsSelf.pos).Normalize();
		Core::Math::Vec3 directionOther = (other.waypoints[0] - trsOther.pos).Normalize();


		if (directionSelfToOther.Dot(directionSelf) > 0.9) // if they face each other
		{
			std::cout << "Colision face each other\n";
			trsSelf.pos += Core::Math::Vec3{ directionSelf.z, directionSelf.y, -directionSelf.x } *(overlapLength / 2);
			trsOther.pos += Core::Math::Vec3{ directionOther.z, directionOther.y, -directionOther.x } *(overlapLength / 2);

		}
		else // they colidde side by side
		{
			std::cout << "Colision Side by Side\n";
			//fix strange behavior for now
			//trsSelf.pos += -directionSelfToOther * (overlapLength / 2);
			//trsOther.pos += directionSelfToOther * (overlapLength / 2);
			trsOther.pos += directionSelfToOther * (overlapLength);
		}

		trsSelf.trsHasChanged = true;
		trsOther.trsHasChanged = true;

	}
	//Priority Low
	else if ((state == CGPMOVE_STATE::E_PUSHED && other.state == CGPMOVE_STATE::E_MOVING) ||
		(state == CGPMOVE_STATE::E_STATIC && other.state == CGPMOVE_STATE::E_MOVING) ||
		(state == CGPMOVE_STATE::E_STATIC && other.state == CGPMOVE_STATE::E_PUSHED) ||
		(state == CGPMOVE_STATE::E_REACH_GOAL && other.state == CGPMOVE_STATE::E_MOVING) ||
		(state == CGPMOVE_STATE::E_REACH_GOAL && other.state == CGPMOVE_STATE::E_PUSHED) ||
		(state == CGPMOVE_STATE::E_REACH_GOAL && other.state == CGPMOVE_STATE::E_REACH_GOAL && reachGoalCountdown < other.reachGoalCountdown))
	{
		if (state == CGPMOVE_STATE::E_STATIC)
		{
			state = CGPMOVE_STATE::E_PUSHED;
			posBeforePushed = trsSelf.pos;
		}
		pushedCooldownBeforeReturn = CGPMOVE_CD_BEFORE_RETURN;

		Core::Math::Vec3 direction = (trsSelf.pos - trsOther.pos).Normalize();
		trsSelf.pos = trsOther.pos + direction * (radius + other.radius);
		trsSelf.trsHasChanged = true;
	}

}

void CGPMove::DrawPath(Render::DebugRenderer& debug, ECS::ComponentTransform& trs)
{
	if (waypoints.size() != 0)
		debug.AddDebugElement(Core::Primitives::CreateLine({ trs.pos.x, 1, trs.pos.z }, { waypoints[0].x, 1, waypoints[0].z }, 0x00FFFF, 0x00FFFF));

	for (int i = 1; i < waypoints.size(); ++i)
	{
		//use 1 for Y so the debug will not be mix up with the map
		debug.AddDebugElement(Core::Primitives::CreateLine({ waypoints[i - 1].x, 1, waypoints[i - 1].z }, { waypoints[i].x, 1, waypoints[i].z }, 0x00FFFF, 0xFF0000));
	}
}