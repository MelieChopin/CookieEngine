#ifndef __MAP_HPP__
#define __MAP_HPP__

#include "ComponentTransform.hpp"
#include "ComponentModel.hpp"
#include "ComponentPhysics.hpp"

#include <array>
#include <string>
#include <algorithm>

struct ID3D11Buffer;

namespace Cookie
{
	namespace Render
	{
		class DebugRenderer;
	}

	namespace Resources
	{
		

		struct Tile
		{
			bool isObstacle = false;
			bool isVisited = false;

			float f = 0;
			float g = 0;
			float h = 0;
			Core::Math::Vec2 pos = { { 0, 0 } };

			std::vector<Tile*> neighbours;
			Tile* parent = nullptr;
		};

		#define MAP_DEFAULT_TILESNB_WIDTH  100
		#define MAP_DEFAULT_TILESNB_HEIGHT 100
		#define MAP_DEFAULT_SCALE_WIDTH    100
		#define MAP_DEFAULT_SCALE_HEIGHT   100

		class Map
		{
		public:
			Core::Math::Vec2 tilesNb      = {{ MAP_DEFAULT_TILESNB_WIDTH, MAP_DEFAULT_TILESNB_HEIGHT }};
			Core::Math::Vec2 tilesSize    = {{ 0, 0 }};

			ECS::ComponentTransform trs;
			ECS::ComponentModel	    model;
			ECS::ComponentPhysics	physic;

			std::array<Tile, MAP_DEFAULT_TILESNB_WIDTH * MAP_DEFAULT_TILESNB_HEIGHT> tiles;
			Tile* tileStart = nullptr;
			Tile* tileEnd = nullptr;

			ECS::ComponentModel	    modelTileStart;
			ECS::ComponentModel	    modelTileEnd;
			ECS::ComponentModel	    modelTileObstacle;


			Map();
			~Map() {}

			void InitTiles();
			int GetTileIndex(Core::Math::Vec2& mousePos);
			Core::Math::Vec2 GetCenterOfBuilding(Core::Math::Vec2& mousePos, Core::Math::Vec2& buildingNbOfTiles);

			bool ApplyPathfinding();

			void Draw(const Core::Math::Mat4& viewProj, ID3D11Buffer** buffer);
			void DrawSpecificTiles(const Core::Math::Mat4& viewProj, ID3D11Buffer** buffer);
			void DrawPath(Render::DebugRenderer& debug);

		};


	}
}

#endif // !__MAP_HPP__
