#include "Physics/PhysicsHandle.hpp"
#include "Core/Primitives.hpp"
#include "Render/DebugRenderer.hpp"
#include "Resources/Map.hpp"

#include <list>

using namespace Cookie::Resources;
using namespace Cookie::Core::Math;

Map::Map()
{
	trs.scale = { MAP_DEFAULT_SCALE_WIDTH, 0.01, MAP_DEFAULT_SCALE_HEIGHT };
	trs.ComputeTRS();
	tilesSize = { { trs.scale.x / tilesNb.x, trs.scale.z / tilesNb.y } };

	//init Component Physic
	physic.physBody = Physics::PhysicsHandle::physSim->createRigidBody(rp3d::Transform(rp3d::Vector3(0.0, 0.0, 0.0), rp3d::Quaternion::identity()));
	physic.physBody->setType(rp3d::BodyType::STATIC);
	physic.AddCubeCollider(trs.scale / 2.f, trs.pos, trs.rot);

	InitTiles();
}

void Map::InitTiles()
{
	for (int x = 0; x < tilesNb.x; x++)
		for (int y = 0; y < tilesNb.y; y++)
		{
			Tile& currentTile = tiles[x + y * tilesNb.x];
			currentTile.pos = { { x * tilesSize.x + tilesSize.x / 2 - trs.scale.x / 2, y * tilesSize.y + tilesSize.y / 2 - trs.scale.z / 2} };

			//Connect to neighbours
			{
				//adjacent
				if (y > 0)
					currentTile.neighbours.push_back(&tiles[x + (y - 1) * tilesNb.x]);
				if (y < tilesNb.y - 1)
					currentTile.neighbours.push_back(&tiles[x + (y + 1) * tilesNb.x]);
				if (x > 0)
					currentTile.neighbours.push_back(&tiles[(x - 1) + y * tilesNb.x]);
				if (x < tilesNb.x - 1)
					currentTile.neighbours.push_back(&tiles[(x + 1) + y * tilesNb.x]);
				
				//diagonal
				if (x > 0 && y > 0)
					currentTile.neighbours.push_back(&tiles[(x - 1) + (y - 1) * tilesNb.x]);
				if (x > 0 && y < tilesNb.y - 1)
					currentTile.neighbours.push_back(&tiles[(x - 1) + (y + 1) * tilesNb.x]);
				if (x < tilesNb.x - 1 && y > 0)
					currentTile.neighbours.push_back(&tiles[(x + 1) + (y - 1) * tilesNb.x]);
				if (x < tilesNb.x - 1 && y < tilesNb.y - 1)
					currentTile.neighbours.push_back(&tiles[(x + 1) + (y + 1) * tilesNb.x]);
			}

		}

}
int  Map::GetTileIndex(Vec2& mousePos)
{
	Vec2 unsignedMousePos{ {mousePos.x + trs.scale.x / 2, mousePos.y + trs.scale.z / 2} };

	return int(unsignedMousePos.x / tilesSize.x) + tilesNb.x * int(unsignedMousePos.y / tilesSize.x);
}
int  Map::GetTileIndex(Vec3& pos)
{
	//helper, same as GetTileIndex(Vec2) juste use Vec3.z instead of Vec2.y

	Vec2 unsignedPos{ {pos.x + trs.scale.x / 2, pos.z + trs.scale.z / 2} };

	return int(unsignedPos.x / tilesSize.x) + tilesNb.x * int(unsignedPos.y / tilesSize.x);
}
Tile& Map::GetTile(Core::Math::Vec2& mousePos)
{
	return tiles[GetTileIndex(mousePos)];
}
Tile& Map::GetTile(Core::Math::Vec3& pos)
{
	return tiles[GetTileIndex(pos)];
}

Vec2 Map::GetCenterOfBuilding(Vec2& mousePos, Vec2& buildingNbOfTiles)
{
	Vec2 tilePos = tiles[GetTileIndex(mousePos)].pos;

	//if buildingNbOfTiles has peer values add half tileSize for each
	tilePos += { {(int)(buildingNbOfTiles.x + 1) % 2 * tilesSize.x / 2, (int)(buildingNbOfTiles.y + 1) % 2 * tilesSize.y / 2}};

	return { {std::clamp(tilePos.x, -trs.scale.x / 2 + buildingNbOfTiles.x * tilesSize.x / 2, trs.scale.x / 2 - buildingNbOfTiles.x * tilesSize.x / 2),
			 std::clamp(tilePos.y, -trs.scale.z / 2 + buildingNbOfTiles.y * tilesSize.y / 2, trs.scale.z / 2 - buildingNbOfTiles.y * tilesSize.y / 2)} };

}

bool Map::ApplyPathfinding(Tile& tileStart, Tile& tileEnd)
{
	if (tileEnd.isObstacle)
		return false;
	//if we are already on the tile end return true without doing any calculation
	if (&tileStart == &tileEnd)
		return true;

	// Set all Tiles to default 
	for (int x = 0; x < tilesNb.x; x++)
		for (int y = 0; y < tilesNb.y; y++)
		{
			Tile& currentTile = tiles[x + y * tilesNb.x];

			currentTile.isVisited = false;
			currentTile.f = INFINITY;
			currentTile.g = INFINITY;
			currentTile.h = INFINITY;
			currentTile.parent = nullptr;
		}

	// Setup starting conditions
	tileStart.g = 0.0f;
	tileStart.h = (tileEnd.pos - tileStart.pos).Length();
	tileStart.f = tileStart.g + tileStart.h;
	std::list<Tile*> listNotTestedTiles;
	listNotTestedTiles.push_back(&tileStart);
	Tile* currentTile = &tileStart;


	while (!listNotTestedTiles.empty() && currentTile != &tileEnd)
	{
		// Sort list of Tiles by lower f
		listNotTestedTiles.sort([](const Tile* firstTile, const Tile* secondTile) { return firstTile->f < secondTile->f; });

		// remove visited Tiles
		while (!listNotTestedTiles.empty() && listNotTestedTiles.front()->isVisited)
			listNotTestedTiles.pop_front();

		// abort if the list is empty
		if (listNotTestedTiles.empty())
			return false;


		currentTile = listNotTestedTiles.front();
		currentTile->isVisited = true;


		// Check each of this node's neighbours...
		for (Tile* currentNeighbour : currentTile->neighbours)
		{
			//Add currentNeighbour to the list only if not visited and not an obstacle
			if (!currentNeighbour->isVisited && !currentNeighbour->isObstacle)
				listNotTestedTiles.push_back(currentNeighbour);

			// Calculate neighbour g value if currentTile become his parent
			float possiblyNewGValue = currentTile->g + (currentTile->pos - currentNeighbour->pos).Length();

			//if it is shorter to have CurrentTile as parent update values of currentNeighbour
			if (possiblyNewGValue < currentNeighbour->g)
			{
				currentNeighbour->parent = currentTile;
				currentNeighbour->g = possiblyNewGValue;
				currentNeighbour->h = (currentNeighbour->pos - tileEnd.pos).Length();
				currentNeighbour->f = currentNeighbour->g + currentNeighbour->h;
			}
		}
	}

	return true;
}

void Map::Draw(const Mat4& viewProj)
{
	model.Draw(viewProj, trs.TRS);
}
void Map::DrawSpecificTiles(const Mat4& viewProj)
{
	for (int x = 0; x < tilesNb.x; x++)
		for (int y = 0; y < tilesNb.y; y++)
		{
			Tile& currentTile = tiles[x + y * tilesNb.x];

			if (currentTile.isObstacle)
				modelTileObstacle.Draw(viewProj, Mat4::TRS({ currentTile.pos.x, 1, currentTile.pos.y }, { 0, 0, 0 }, { tilesSize.x, 1, tilesSize.y }));
		}

}