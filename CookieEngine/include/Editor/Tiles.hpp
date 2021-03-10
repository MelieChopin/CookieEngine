#ifndef __TILES_HPP__
#define __TILES_HPP__

#include <vector>

namespace Cookie
{
	namespace Editor
	{
		struct TileProp
		{
			int idEntity = -1; //num of index block : -1 == none
			bool fill = false;
		};

		class Tiles
		{
		public:
			std::vector<TileProp> tiles;
			float widthTileProp;
			float lengthTileProp;
			int widthTile;
			int lengthTile;

			Tiles(int _widthTile = 15, int _lengthTile = 15, float _widthTileProp = 1, float _lengthTileProp = 1);
			~Tiles();
		};
	}
}

#endif // !__TILES_HPP__
