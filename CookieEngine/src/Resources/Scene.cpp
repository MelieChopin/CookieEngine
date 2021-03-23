#include <reactphysics3d/reactphysics3d.h>
#include "Coordinator.hpp"
#include "Resources/Scene.hpp"
#include "Vec3.hpp"
#include <iostream>

using namespace Cookie::Resources;
using namespace Cookie::Core::Math;

#define EPSILON 1/4096

Scene::Scene(const std::shared_ptr<reactphysics3d::PhysicsCommon>& physCom):
	physSim{physCom}
{
	int length = 15;
	int width = 15;
	int widthTile = 6;
	int depthTile = 6;
	tiles = Resources::Tiles(width, length, widthTile, depthTile);
	//
	//Cookie::ECS::Coordinator::AddEntity(entityHandler, componentHandler, SIGNATURE_TRANSFORM + SIGNATURE_MODEL, resources, "Map");
	//componentHandler.componentModels[0].mesh = resources.GetMesh("Quad");
	//componentHandler.componentTransforms[0].localTRS.pos = Vec3(0, 0, 0);
	//componentHandler.componentTransforms[0].localTRS.rot = Vec3(90.0f, 0, 0);
	//componentHandler.componentTransforms[0].localTRS.scale = Vec3(tiles.widthTile * tiles.widthTileProp / 2, tiles.depthTile * tiles.depthTileProp / 2, 0);
	//entityHandler.entities[0].tag = "MAP";
}

Scene::~Scene()
{

}

void Scene::InitCoordinator(Cookie::ECS::Coordinator& coordinator)
{
	coordinator.entityHandler = &entityHandler;
	coordinator.componentHandler = &componentHandler;
}


//void Scene::ResizeSizeTilesWithScaleOfTheMap(float newWidthPlane, float newLengthPlane)
//{
//	std::cout << "BEFORE " << tiles.widthTileProp << " " << tiles.depthTileProp << "\n";
//	tiles.widthTileProp = 2 * newWidthPlane / tiles.widthTile;
//	tiles.depthTileProp = 2 * newLengthPlane / tiles.depthTile;
//	std::cout << "AFTER " << tiles.widthTileProp << " " << tiles.depthTileProp << "\n";
//}

void Scene::ChangeNumberOfTiles(int newSizeWidthTile, int newSizeLengthTile)
{
	if (newSizeLengthTile == tiles.depthTile && newSizeWidthTile == tiles.widthTile)
		return;

	if (newSizeLengthTile > tiles.depthTile)
	{
		for (int i = 0; i < tiles.widthTile * (newSizeLengthTile - tiles.depthTile); i++)
			tiles.tiles.push_back(Resources::TileProp());

		tiles.depthTile = newSizeLengthTile;
	}
	else if (newSizeLengthTile < tiles.depthTile)
	{
		tiles.tiles.erase(tiles.tiles.begin() + (tiles.widthTile) * (tiles.depthTile - (tiles.depthTile - newSizeLengthTile)), tiles.tiles.end());
		tiles.depthTile = newSizeLengthTile;
	}

	if (newSizeWidthTile > tiles.widthTile)
	{
		for (int i = tiles.depthTile; i > 0; i--)
			tiles.tiles.insert(tiles.tiles.begin() + i  * tiles.widthTile - 1, newSizeWidthTile - tiles.widthTile , Resources::TileProp());

		tiles.widthTile = newSizeWidthTile;
	}
	else if (newSizeWidthTile < tiles.widthTile)
	{
		for (int i = tiles.depthTile; i > 0; i--)
			tiles.tiles.erase(tiles.tiles.begin() + i * tiles.widthTile - (tiles.widthTile - newSizeWidthTile), tiles.tiles.begin() + i * tiles.widthTile);
			
		tiles.widthTile = newSizeWidthTile;
	}
}