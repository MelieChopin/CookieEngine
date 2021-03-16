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
	Cookie::ECS::Coordinator::AddEntity(entityHandler, componentHandler, SIGNATURE_TRANSFORM + SIGNATURE_MODEL, resources, "Map");
	componentHandler.componentModels[0].mesh = resources.GetMesh("Quad");
	componentHandler.componentTransforms[0].localTRS.rot = Vec3(-PI / 2.0f,0,0);
	componentHandler.componentTransforms[0].localTRS.scale = Vec3(15, 15, 0);
	entityHandler.entities[0].tag = "MAP";
	plane = { Vec3(0, 1, 0), 0 };
	widthPlane = componentHandler.componentTransforms[0].localTRS.scale.x;
	lengthPlane = componentHandler.componentTransforms[0].localTRS.scale.y;
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
		

	/*int indexWid = temp.x / (widthTile);
	int indexLen = temp.z / (lengthTile);

	std::cout << indexWid << " " << indexLen << std::endl;

	pointCollision.x = indexWid * widthTile - lenghtMap + widthTile / 2;
	pointCollision.z = indexLen * lengthTile - lenghtMap + lengthTile / 2;*/

	return true;
}