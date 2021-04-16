#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "Vec2.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentPhysics.hpp"
#include "ResourcesManager.hpp"

#include <string>
#include <algorithm>

namespace Cookie
{
	namespace Resources
	{
		class Map
		{
		public:
			Core::Math::Vec2 tilesNb      = {{ 0, 0 }};
			Core::Math::Vec2 tilesSize    = {{ 0, 0 }};
			Core::Math::Vec2 tilesNbIcons = {{ 2, 2 }};

			ECS::ComponentTransform trs;
			ECS::ComponentModel	    model;
			ECS::ComponentPhysics	physic;


			Map()
			{				
				trs.scale = {100, 100, 0.01};
				trs.rot = {90, 0, 0};

				tilesNb = {{ 10, 10 }};
				tilesSize = {{ trs.scale.x * 2 / tilesNb.x, trs.scale.y * 2 / tilesNb.y }};

				physic.physBody = Physics::PhysicsHandle::physSim->createRigidBody(rp3d::Transform(rp3d::Vector3(0.0, 0.0, 0.0), rp3d::Quaternion::identity()));
				physic.physBody->setType(rp3d::BodyType::STATIC);
				physic.AddCubeCollider(trs.scale, trs.pos, trs.rot);

			}
			~Map() {}

			Core::Math::Vec2 GetCenterOfBuilding(Core::Math::Vec2& mousePos, Core::Math::Vec2& buildingNbOfTiles)
			{
				Core::Math::Vec2 unsignedMousePos {{mousePos.x + trs.scale.x, mousePos.y + trs.scale.y}};
				
				Core::Math::Vec2 centerOfBuilding {{tilesSize.x * (int)(unsignedMousePos.x / tilesSize.x) + (2 - (int)buildingNbOfTiles.x % 2) * tilesSize.x / 2,
												    tilesSize.y * (int)(unsignedMousePos.y / tilesSize.y) + (2 - (int)buildingNbOfTiles.y % 2) * tilesSize.y / 2} };

				return {{std::clamp(centerOfBuilding.x, buildingNbOfTiles.x * tilesSize.x / 2, trs.scale.x * 2 - buildingNbOfTiles.x * tilesSize.x / 2) - trs.scale.x,
						 std::clamp(centerOfBuilding.y, buildingNbOfTiles.y * tilesSize.y / 2, trs.scale.y * 2 - buildingNbOfTiles.y * tilesSize.y / 2) - trs.scale.y}};


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


		};


	}
}

#endif // !__MAP_HPP__
