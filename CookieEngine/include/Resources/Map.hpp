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
			std::vector<Tile> tiles;

			ECS::ComponentTransform trs;
			ECS::ComponentModel	    model;
			ECS::ComponentPhysics	physic;

			//will be removed
			ECS::ComponentModel	    modelTileObstacle;


			Map();
			~Map() {}

			void InitTiles();
			void ScaleHasChanged();
			void TileNbHasChanged();

			void ResetTilesTempObstacles();
			int GetTileIndex(Core::Math::Vec2& mousePos);
			int GetTileIndex(Core::Math::Vec3& pos);
			Tile& GetTile(Core::Math::Vec2& mousePos);
			Tile& GetTile(Core::Math::Vec3& pos);
			Core::Math::Vec2 GetCenterOfBuilding(Core::Math::Vec2& mousePos, Core::Math::Vec2& buildingNbOfTiles);

			bool ApplyPathfinding(Tile& tileStart, Tile& tileEnd);

			void Draw(const Core::Math::Mat4& proj, const Core::Math::Mat4& view, ID3D11Buffer** CBuffer);
			void DrawSpecificTiles(const Core::Math::Mat4& proj, const Core::Math::Mat4& view, ID3D11Buffer** CBuffer);


		};


	}
}

#endif // !__MAP_HPP__
