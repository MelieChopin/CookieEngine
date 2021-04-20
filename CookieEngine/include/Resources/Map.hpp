#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "Vec2.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentPhysics.hpp"
#include "ResourcesManager.hpp"
#include "Core/Primitives.hpp"
#include "Render/DebugRenderer.hpp"

#include <string>
#include <algorithm>


namespace Cookie
{
	namespace Resources
	{
		class Map
		{
		public:
			Core::Math::Vec2 tilesNb      = {{ 10, 10 }};
			Core::Math::Vec2 tilesSize    = {{ 0, 0 }};
			Core::Math::Vec2 tilesNbIcons = {{ 2, 2 }};

			ECS::ComponentTransform trs;
			ECS::ComponentModel	    model;
			ECS::ComponentPhysics	physic;


			struct Tile
			{
				bool isObstacle = false;			
				bool isVisited = false;		

				float f = 0;
				float g = 0;
				float h = 0;		
				Core::Math::Vec2 pos   = {{ 0, 0 }};

				std::vector<Tile*> neighbours;
				Tile*			   parent = nullptr;					    
			};

			std::array<Tile, 10 * 10> tiles;
			Tile* tileStart = nullptr;
			Tile* tileEnd = nullptr;

			ECS::ComponentModel	    modelTileStart;
			ECS::ComponentModel	    modelTileEnd;
			ECS::ComponentModel	    modelTileObstacle;



			Map()
			{				
				trs.scale = {100, 100, 0.01};
				trs.rot = {90, 0, 0};
				trs.ComputeTRS();
				tilesSize = { { trs.scale.x / tilesNb.x, trs.scale.y / tilesNb.y } };

				physic.physBody = Physics::PhysicsHandle::physSim->createRigidBody(rp3d::Transform(rp3d::Vector3(0.0, 0.0, 0.0), rp3d::Quaternion::identity()));
				physic.physBody->setType(rp3d::BodyType::STATIC);
				physic.AddCubeCollider(trs.scale / 2.f, trs.pos, trs.rot);

				//init Tiles
				for (int x = 0; x < tilesNb.x; x++)
					for (int y = 0; y < tilesNb.y; y++)
					{
						Tile& currentTile = tiles[x + y * tilesNb.x];
						currentTile.pos = { { x * tilesSize.x + tilesSize.x / 2 - trs.scale.x / 2, y * tilesSize.y + tilesSize.y / 2 - trs.scale.y / 2} };
				
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
			~Map() {}

			int GetTileIndex(Core::Math::Vec2& mousePos)
			{
				Core::Math::Vec2 unsignedMousePos{ {mousePos.x + trs.scale.x / 2, mousePos.y + trs.scale.y / 2} };

				return int(unsignedMousePos.x / tilesSize.x) + tilesNb.x * int(unsignedMousePos.y / tilesSize.x);
			}
			Core::Math::Vec2 GetCenterOfBuilding(Core::Math::Vec2& mousePos, Core::Math::Vec2& buildingNbOfTiles)
			{
				Core::Math::Vec2 tilePos = tiles[GetTileIndex(mousePos)].pos;

				//if buildingNbOfTiles has peer values add half tileSize for each
				tilePos += { {(int)(buildingNbOfTiles.x + 1) % 2 * tilesSize.x / 2, (int)(buildingNbOfTiles.y + 1) % 2 * tilesSize.y / 2}};

				return {{std::clamp(tilePos.x, -trs.scale.x / 2 + buildingNbOfTiles.x * tilesSize.x / 2, trs.scale.x / 2 - buildingNbOfTiles.x * tilesSize.x / 2),
						 std::clamp(tilePos.y, -trs.scale.y / 2 + buildingNbOfTiles.y * tilesSize.y / 2, trs.scale.y / 2 - buildingNbOfTiles.y * tilesSize.y / 2)}};

			}
			void DisplayTilesIcons(Core::Math::Vec2& mousePos, const Core::Math::Vec2& buildingNbOfTiles)
			{
				//Core::Math::Vec2 buildingTopLeftCorner = { {mousePos.x - ((int)mousePos.x % (int)tilesSize.x) - ((int(buildingNbOfTiles.x - 1) / 2) * tilesSize.x),
				//										  mousePos.y - ((int)mousePos.y % (int)tilesSize.y) - ((int(buildingNbOfTiles.y - 1) / 2) * tilesSize.y)} };
				//
				//Core::Math::Vec2 iconsSquareTopLeftCorner = { {buildingTopLeftCorner.x - tilesNbIcons.x * 2 * tilesSize.x,
				//											 buildingTopLeftCorner.y - tilesNbIcons.y * 2 * tilesSize.y} };
				//
				//for (int i = 0; i < buildingNbOfTiles.x + tilesNbIcons.x * 2; ++i)
				//	for (int j = 0; j < buildingNbOfTiles.y + tilesNbIcons.y * 2; ++j)
				//	{
				//		//Core::Math::Vec2 iconPos = iconsSquareTopLeftCorner + Core::Math::Vec2{tilesSize.x* i, tilesSize.y* j};
				//		//Display Icon at iconPos
				//	}
				//
				//
			}

			bool ApplyPathfinding()
			{
				if (tileStart == nullptr || tileEnd == nullptr)
					return false;

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
				tileStart->g = 0.0f;
				tileStart->h = (tileEnd->pos - tileStart->pos).Length();
				tileStart->f = tileStart->g + tileStart->h;
				std::list<Tile*> listNotTestedTiles;
				listNotTestedTiles.push_back(tileStart);
				Tile* currentTile = tileStart;


				while (!listNotTestedTiles.empty() && currentTile != tileEnd)
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
							currentNeighbour->h = (currentNeighbour->pos - tileEnd->pos).Length();
							currentNeighbour->f = currentNeighbour->g + currentNeighbour->h;
						}
					}
				}

				return true;
			}


			void Draw(const Core::Math::Mat4& viewProj)
			{
				model.Draw(viewProj, trs.TRS);
			}
			void DrawSpecificTiles(const Core::Math::Mat4& viewProj)
			{
				if(tileStart)
					modelTileStart.Draw(viewProj, Core::Math::Mat4::TRS({ tileStart->pos.x, 1, tileStart->pos.y }, { 0, 0, 0 }, { tilesSize.x, 1, tilesSize.y }));
				if(tileEnd)
					modelTileEnd.Draw(viewProj, Core::Math::Mat4::TRS({ tileEnd->pos.x, 1, tileEnd->pos.y }, { 0, 0, 0 }, { tilesSize.x, 1, tilesSize.y }));

				for (int x = 0; x < tilesNb.x; x++)
					for (int y = 0; y < tilesNb.y; y++)
					{
						Tile& currentTile = tiles[x + y * tilesNb.x];

						if (currentTile.isObstacle)
							modelTileObstacle.Draw(viewProj, Core::Math::Mat4::TRS({currentTile.pos.x, 1, currentTile.pos.y}, {0, 0, 0}, {tilesSize.x, 1, tilesSize.y }));
					}

			}
			void DrawPath(Render::DebugRenderer& debug)
			{
				if (tileEnd != nullptr)
				{
					Tile* currentTile = tileEnd;

					while (currentTile->parent != nullptr)
					{
						debug.AddDebugElement(Core::Primitives::CreateLine({ currentTile->pos.x, 1, currentTile->pos.y }, { currentTile->parent->pos.x, 1, currentTile->parent->pos.y }, 0x00FFFF, 0x00FFFF));
						currentTile = currentTile->parent;
					}
				}
			}

		};


	}
}

#endif // !__MAP_HPP__
