#include "Scene.hpp"
#include "Vec3.hpp"
#include <iostream>

using namespace Cookie::Editor;
using namespace Cookie::Core::Math;

#define EPSILON 1/4096

Scene::Scene()
{
	entityHandler = Cookie::ECS::EntityHandler();
	componentHandler = Cookie::ECS::ComponentHandler();
}

Scene::Scene(const Resources::ResourcesManager& resources, Cookie::ECS::Coordinator& coordinator)
{
	int length = 15;
	int width = 15;
	int widthTile = 6;
	int depthTile = 6;
	tiles = Tiles(width, length, widthTile, depthTile);

	Cookie::ECS::Coordinator::AddEntity(entityHandler, componentHandler, SIGNATURE_TRANSFORM + SIGNATURE_MODEL, resources, "Map");
	componentHandler.componentModels[0].mesh = resources.GetMesh("Quad");
	componentHandler.componentTransforms[0].localTRS.pos = Vec3(0, 0, 0);
	componentHandler.componentTransforms[0].localTRS.rot = Vec3(90.0f, 0, 0);
	componentHandler.componentTransforms[0].localTRS.scale = Vec3(tiles.widthTile * tiles.widthTileProp / 2, tiles.depthTile * tiles.depthTileProp / 2, 0);
	entityHandler.entities[0].tag = "MAP";
	
	plane = { Vec3(0, 1, 0), 0 };
	widthPlane = tiles.widthTile * tiles.widthTileProp / 2;
	lengthPlane = tiles.depthTile * tiles.depthTileProp / 2;
}

Scene::Scene(const Scene& _scene)
{
	*this = _scene;
}

Scene::~Scene()
{

}

void Scene::InitCoordinator(Cookie::ECS::Coordinator& coordinator)
{
	coordinator.entityHandler = &entityHandler;
	coordinator.componentHandler = &componentHandler;
}


void Scene::ResizeSizeTilesWithScaleOfTheMap(float newWidthPlane, float newLengthPlane)
{
	std::cout << "BEFORE " << tiles.widthTileProp << " " << tiles.depthTileProp << "\n";
	tiles.widthTileProp = 2 * newWidthPlane / tiles.widthTile;
	tiles.depthTileProp = 2 * newLengthPlane / tiles.depthTile;
	std::cout << "AFTER " << tiles.widthTileProp << " " << tiles.depthTileProp << "\n";
}

void Scene::ChangeNumberOfTiles(int newSizeWidthTile, int newSizeLengthTile)
{
	if (newSizeLengthTile == tiles.depthTile && newSizeWidthTile == tiles.widthTile)
		return;

	if (newSizeLengthTile > tiles.depthTile)
	{
		for (int i = 0; i < tiles.widthTile * (newSizeLengthTile - tiles.depthTile); i++)
			tiles.tiles.push_back(TileProp());

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
			tiles.tiles.insert(tiles.tiles.begin() + i  * tiles.widthTile - 1, newSizeWidthTile - tiles.widthTile , TileProp());

		tiles.widthTile = newSizeWidthTile;
	}
	else if (newSizeWidthTile < tiles.widthTile)
	{
		for (int i = tiles.depthTile; i > 0; i--)
			tiles.tiles.erase(tiles.tiles.begin() + i * tiles.widthTile - (tiles.widthTile - newSizeWidthTile), tiles.tiles.begin() + i * tiles.widthTile);
			
		tiles.widthTile = newSizeWidthTile;
	}
	ResizeSizeTilesWithScaleOfTheMap(widthPlane, lengthPlane);
}

bool Scene::LinePlane(Cookie::Core::Math::Vec3& pointCollision, const Cookie::Core::Math::Vec3& firstPoint, const Cookie::Core::Math::Vec3& secondPoint)
{
	Vec3 line = secondPoint - firstPoint;
	if (line.Dot(plane.norm) <= EPSILON && line.Dot(plane.norm) >= -EPSILON)
		return false;

	float t = ((plane.dist - plane.norm.Dot(firstPoint)) / (plane.norm.Dot(line)));

	if (t <= 0)
	{
		pointCollision = { FLT_MAX, FLT_MAX, FLT_MAX };
		return false;
	}

	pointCollision = firstPoint + line * t;

	if (componentHandler.componentTransforms.size() > 0)
	{
		widthPlane = componentHandler.componentTransforms[0].localTRS.scale.x;
		lengthPlane = componentHandler.componentTransforms[0].localTRS.scale.y;
	}
	
	Vec3 temp = pointCollision;
	temp.x += widthPlane;
	temp.z += lengthPlane;

	if (temp.x < 0 || temp.x > widthPlane * 2 ||
		temp.z < 0 || temp.z > lengthPlane * 2)
	{
		pointCollision = { FLT_MAX, FLT_MAX, FLT_MAX };
		return false;
	}

	///Check the tiles

	int indexWidthOfTiles = temp.x / (tiles.widthTileProp);
	int indexLengthOfTiles = temp.z / (tiles.depthTileProp);

	std::cout << "INDEX TILES   " << indexWidthOfTiles << " " << indexLengthOfTiles << "\n";

	///

	return true;
}