#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "ECS/Coordinator.hpp"
#include "Vec3.hpp"
#include "Tiles.hpp"


//temp
#include <iostream>

namespace Cookie
{
	namespace Editor
	{
		//To cut
		struct Plane
		{
			Cookie::Core::Math::Vec3 norm;
			float dist;
		};

		#define MaxScene 20 

		class Scene
		{
		public:
			Cookie::ECS::EntityHandler		entityHandler;
			Cookie::ECS::ComponentHandler	componentHandler;
			Tiles							tiles;
			std::string						name = "Scene";
			std::string						filepath;
			Plane							plane;
			float							widthPlane;
			float							lengthPlane;

		public:
			Scene();
			Scene(const Resources::ResourcesManager& resources, Cookie::ECS::Coordinator& coordinator);
			Scene(const Scene& _scene);
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

				int indexWidthOfTiles = (componentHandler.componentTransforms[index].localTRS.pos.x + widthPlane) / (tiles.widthTileProp);
				int indexLengthOfTiles = (componentHandler.componentTransforms[index].localTRS.pos.z + lengthPlane) / (tiles.lengthTileProp);

				componentHandler.componentTransforms[index].localTRS.pos.x = indexWidthOfTiles * (tiles.widthTileProp) - widthPlane + tiles.widthTileProp / 2;
				componentHandler.componentTransforms[index].localTRS.pos.z = indexLengthOfTiles * (tiles.lengthTileProp) - lengthPlane + tiles.lengthTileProp / 2;

				for (int i = 0; i < tiles.tiles.size(); i++)
				{
					if (tiles.tiles[i].idEntity == id)
					{
						tiles.tiles[i].idEntity = -1;
						tiles.tiles[i].fill = false;
						break;
					}
				}

				tiles.tiles[indexWidthOfTiles + indexLengthOfTiles * tiles.widthTile].fill = true;
				tiles.tiles[indexWidthOfTiles + indexLengthOfTiles * tiles.widthTile].idEntity = id;

			}
			/// </Temp>

			void InitCoordinator(Cookie::ECS::Coordinator& coordinator);
			void ResizeSizeTilesWithScaleOfTheMap(float newWidthPlane, float newLengthPlane);
			void ChangeNumberOfTiles(int newSizeWidthTile, int newSizeLengthTile);
			void ChangeName(const char* newName) { name = newName; }
			bool LinePlane(Cookie::Core::Math::Vec3& pointCollision, const Cookie::Core::Math::Vec3& firstPoint, const Cookie::Core::Math::Vec3& secondPoint);
		};
	}
}

#endif // !__SCENE_HPP__
