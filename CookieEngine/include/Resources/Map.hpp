#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "Vec2.hpp"
#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentPhysics.hpp"

#include <string>
#include <algorithm>

namespace Cookie
{
	namespace Resources
	{
		class Map
		{
		public:
			Core::Math::Vec2 tilesNb      = { 0, 0 };
			Core::Math::Vec2 tilesSize    = { 0, 0 };
			Core::Math::Vec2 tilesNbIcons = { 2, 2 };

			ECS::ComponentTransform trs;
			ECS::ComponentModel	    model;
			ECS::ComponentPhysics	physic;


			Map() {}
			~Map() {}

			Core::Math::Vec2 GetCenterOfBuilding(Core::Math::Vec2& mousePos, const Core::Math::Vec2& buildingNbOfTiles)
			{
				return {mousePos.x - ((int)mousePos.x % (int)tilesSize.x) + (tilesSize.x / buildingNbOfTiles.x),
						mousePos.y - ((int)mousePos.y % (int)tilesSize.y) + (tilesSize.y / buildingNbOfTiles.y) };
			}

			void DisplayTilesIcons(Core::Math::Vec2& mousePos, const Core::Math::Vec2& buildingNbOfTiles)
			{
				Core::Math::Vec2 buildingTopLeftCorner = {mousePos.x - ((int)mousePos.x % (int)tilesSize.x) - ((int(buildingNbOfTiles.x - 1) / 2) * tilesSize.x),
														  mousePos.y - ((int)mousePos.y % (int)tilesSize.y) - ((int(buildingNbOfTiles.y - 1) / 2) * tilesSize.y)};
			
				Core::Math::Vec2 iconsSquareTopLeftCorner = {buildingTopLeftCorner.x - tilesNbIcons.x * 2 * tilesSize.x,
															 buildingTopLeftCorner.y - tilesNbIcons.y * 2 * tilesSize.y};

				for (int i = 0; i < buildingNbOfTiles.x + tilesNbIcons.x * 2; ++i)
					for (int j = 0; j < buildingNbOfTiles.y + tilesNbIcons.y * 2; ++j)
					{
						Core::Math::Vec2 iconPos = iconsSquareTopLeftCorner + Core::Math::Vec2{tilesSize.x* i, tilesSize.y* j};
						//Display Icon at iconPos
					}

			
			}


		};


	}
}

#endif // !__MAP_HPP__
