#include "Scene.hpp"
#include "Vec3.hpp"

using namespace Cookie::Editor;
using namespace Cookie::Core::Math;

#define EPSILON 1/4096

Scene::Scene()
{
	entityHandler = Cookie::ECS::EntityHandler();
	componentHandler = Cookie::ECS::ComponentHandler();
}

Scene::Scene(const Resources::ResourcesManager& resources)
{
	int length = 30;
	int width = 10;
	tiles = Tiles(width, length);

	for (int i = 0; i < width * length; i++)
	{
		Cookie::ECS::Coordinator::AddEntity(entityHandler, componentHandler, SIGNATURE_TRANSFORM + SIGNATURE_MODEL, resources, "Map");
		componentHandler.componentModels[i].mesh = resources.GetMesh("Quad");
		componentHandler.componentTransforms[i].localTRS.translation = Vec3(i % width, 0, i / length);
		componentHandler.componentTransforms[i].localTRS.rotation = Vec3(PI / 2, 0, 0);
		componentHandler.componentTransforms[i].localTRS.scale = Vec3(0.5, 0.5, 0);
	}
	
	
	entityHandler.entities[0].tag = "MAP";
	plane = { Vec3(0, 1, 0), 0 };
	widthPlane = width / 4;
	lengthPlane = length / 4;
}

Scene::Scene(const Scene& _scene)
{
	*this = _scene;
}

Scene::~Scene()
{

}

void Scene::LoadScene(Cookie::ECS::Coordinator& coordinator)
{
	coordinator.entityHandler = &entityHandler;
	coordinator.componentHandler = &componentHandler;
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

	/*if (componentHandler.componentTransforms.size() > 0)
	{
		widthPlane = componentHandler.componentTransforms[0].localTRS.scale.x;
		lengthPlane = componentHandler.componentTransforms[0].localTRS.scale.y;
	}*/
	
	Vec3 temp = pointCollision;
	temp.x += widthPlane;
	temp.z += lengthPlane;

	if (temp.x < 0 || temp.x > widthPlane * 2 ||
		temp.z < 0 || temp.z > lengthPlane * 2)
	{
		pointCollision = { FLT_MAX, FLT_MAX, FLT_MAX };
		return false;
	}

	return true;
}