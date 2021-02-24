#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "ECS/Coordinator.hpp"
#include "Vec3.hpp"
#include "ECS/Coordinator.hpp"

namespace Cookie
{
	namespace Editor
	{
		struct TileProp
		{
			int index = 0; //num of index block
			bool fill = false;
		};

		//To cut
		struct Plane
		{
			Cookie::Core::Math::Vec3 norm;
			float dist;
		};

		#define MaxScene 20 

		class Scene
		{
		public:
			Cookie::ECS::EntityHandler		entityHandler;
			Cookie::ECS::ComponentHandler	componentHandler;
			Plane							plane;
			float							widthPlane;
			float							lengthPlane;

		public:
			Scene();
			Scene(const Resources::ResourcesManager& resources);
			Scene(const Scene& _scene);
			~Scene();

			void LoadScene(Cookie::ECS::Coordinator& coordinator);
			bool LinePlane(Cookie::Core::Math::Vec3& pointCollision, const Cookie::Core::Math::Vec3& firstPoint, const Cookie::Core::Math::Vec3& secondPoint);
		};
	}
}

#endif // !__SCENE_HPP__
