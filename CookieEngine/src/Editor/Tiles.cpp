#include "Tiles.hpp"

using namespace Cookie::Editor;

Tiles::Tiles(int _widthTileProp, int _lengthTileProp, int _widthTile, int _lengthTile)
{
	for (int i = 0; i < _widthTileProp * _lengthTileProp; i++)
		tiles.push_back(TileProp());

	widthTileProp = _widthTileProp;
	lengthTileProp = _lengthTileProp;
	widthTile = _widthTile;
	lengthTile = _lengthTile;
}

Tiles::~Tiles()
{

}