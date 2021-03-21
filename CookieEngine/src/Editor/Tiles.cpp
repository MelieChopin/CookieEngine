#include "Tiles.hpp"

using namespace Cookie::Editor;

Tiles::Tiles(int _widthTile, int _lengthTile, float _widthTileProp, float _lengthTileProp)
{
	for (int i = 0; i < _widthTile * _lengthTile; i++)
		tiles.push_back(TileProp());	

	widthTileProp = _widthTileProp;
	depthTileProp = _lengthTileProp;
	widthTile = _widthTile;
	depthTile = _lengthTile;
}

Tiles::~Tiles()
{

}


void Tiles::ReInitAllTiles()
{
	for (int i = 0; i < widthTile * depthTile; i++)
	{
		if (!tiles[i].fill)
			continue;
		tiles[i].fill = false;
		tiles[i].idEntity = -1;
	}
}