#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include "ECS/EntityHandler.hpp"
#include "ECS/ComponentHandler.hpp"
#include "Gameplay/ArmyHandler.hpp"
#include "Physics/PhysicsSimulator.hpp"
#include "UI/UIscene.hpp"
#include "Camera.hpp"
#include "Resources/Map.hpp"
#include "Light.hpp"


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
			Gameplay::ArmyHandler					armyHandler;
			Physics::PhysicsSimulator				physSim;
			Map										map;
			UI::UIscene								uiScene;
			std::shared_ptr<Cookie::Render::Camera> camera{nullptr};
			Cookie::Render::LightsArray lights;

		public:
			Scene();
			~Scene();

			void InitCoordinator(Cookie::ECS::Coordinator& coordinator);
			void ChangeName(const char* newName) { name = newName; }
		};
	}
}

#endif // !__SCENE_HPP__
