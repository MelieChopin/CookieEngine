#include "Tiles.hpp"

using namespace Cookie::Editor;

Tiles::Tiles(int _widthTile, int _lengthTile, float _widthTileProp, float _lengthTileProp)
{
	for (int i = 0; i < _widthTile * _lengthTile; i++)
		tiles.push_back(TileProp());	

	widthTileProp = _widthTileProp;
	lengthTileProp = _lengthTileProp;
	widthTile = _widthTile;
	lengthTile = _lengthTile;
}

Tiles::~Tiles()
{

}