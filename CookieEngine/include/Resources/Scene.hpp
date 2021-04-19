#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Physics/PhysicsSimulator.hpp"
#include "UI/UIgame.hpp"
#include "Camera.hpp"
#include "Vec3.hpp"
#include "Map.hpp"


//temp
#include <iostream>

namespace Cookie
{
	namespace ECS
	{
		class Coordinator;
	}

	namespace Resources
	{
		#define MaxScene 20 

		class Scene
		{
		public:
			std::string								name = "Scene";
			std::string								filepath;

			ECS::EntityHandler						entityHandler;
			ECS::ComponentHandler					componentHandler;
			Physics::PhysicsSimulator				physSim;
			Resources::Map							map;
			UI::UIgame								uiGame;
			std::shared_ptr<Cookie::Render::Camera> camera{nullptr};

		public:
			Scene();
			~Scene();

			void InitCoordinator(Cookie::ECS::Coordinator& coordinator);
			void ChangeNumberOfTiles(int newSizeWidthTile, int newSizeLengthTile);
			void ChangeName(const char* newName) { name = newName; }
		};
	}
}

#endif // !__SCENE_HPP__
