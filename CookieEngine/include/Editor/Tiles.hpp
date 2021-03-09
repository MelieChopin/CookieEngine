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
			int widthTileProp;
			int lengthTileProp;
			int widthTile;
			int lengthTile;

			Tiles(int _widthTileProp = 15, int _lengthTileProp = 15, int _widthTile = 1, int _lengthTile = 1);
			~Tiles();
		};
	}
}

#endif // !__TILES_HPP__
