#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Physics/PhysicsSimulator.hpp"
#include "UI/UIgame.hpp"
#include "Camera.hpp"
#include "Vec3.hpp"
#include "Tiles.hpp"
#include "Map.hpp"


//temp
#include <iostream>

namespace Cookie
{
	namespace ECS
	{
		class Coordinator;
	}

	namespace Resources
	{
		#define MaxScene 20 

		class Scene
		{
		public:
			std::string								name = "Scene";
			std::string								filepath;

			ECS::EntityHandler						entityHandler;
			ECS::ComponentHandler					componentHandler;
			Physics::PhysicsSimulator				physSim;
			Resources::Map							map;
			UI::UIgame								uiGame;
			std::shared_ptr<Cookie::Render::Camera> camera{nullptr};
			Resources::Tiles						tiles;//will be map

		public:
			Scene();
			~Scene();

			/// <Temp>
			void ParcourTiles()
			{
				for (int i = 0; i < tiles.tiles.size(); i++)
				{
					std::cout << tiles.tiles[i].fill << " " << i << "\n";
				}
			}

			void AddToTiles(int id)
			{
				int index = -1;
				for (int i = 0; i < entityHandler.entities.size(); i++)
				{
					if (entityHandler.entities[i].id == id)
					{
						index = i;
						break;
					}
				}

				if (index == -1)
					return;

				//int indexWidthOfTiles = (componentHandler.componentTransforms[index].localTRS.pos.x + widthPlane) / (tiles.widthTileProp);
				//int indexLengthOfTiles = (componentHandler.componentTransforms[index].localTRS.pos.z + lengthPlane) / (tiles.depthTileProp);
				//
				//componentHandler.componentTransforms[index].localTRS.pos.x = indexWidthOfTiles * (tiles.widthTileProp) - widthPlane + tiles.widthTileProp / 2;
				//componentHandler.componentTransforms[index].localTRS.pos.z = indexLengthOfTiles * (tiles.depthTileProp) - lengthPlane + tiles.depthTileProp / 2;

				for (int i = 0; i < tiles.tiles.size(); i++)
				{
					if (tiles.tiles[i].idEntity == id)
					{
						tiles.tiles[i].idEntity = -1;
						tiles.tiles[i].fill = false;
						break;
					}
				}

				//tiles.tiles[indexWidthOfTiles + indexLengthOfTiles * tiles.widthTile].fill = true;
				//tiles.tiles[indexWidthOfTiles + indexLengthOfTiles * tiles.widthTile].idEntity = id;
				//
			}
			/// </Temp>

			void InitCoordinator(Cookie::ECS::Coordinator& coordinator);
			void ChangeNumberOfTiles(int newSizeWidthTile, int newSizeLengthTile);
			void ChangeName(const char* newName) { name = newName; }
		};
	}
}

#endif // !__SCENE_HPP__
