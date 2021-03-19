#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <memory>
//#include <reactphysics3d/reactphysics3d.h>
#include "Render/Renderer.hpp"
#include "Core/Window.hpp"
#include "Physics/PhysicsSimulator.hpp"
//#include "Input.hpp"
#include "UIallIn.hpp"
#include "Coordinator.hpp"
#include "Render/Camera.hpp"
#include "Render/FrameBuffer.hpp"
#include "Editor/Scene.hpp"

namespace Cookie
{
	class Engine
	{
		public:
			
			Core::Window				window;
			//Core::Input 				input;
			Render::Renderer			renderer;
			Core::UIeditor				ui;
			Resources::ResourcesManager	resources;
			std::shared_ptr<Render::Camera>	camera = nullptr;
			bool						isFlyCam = false;
			ECS::Coordinator			coordinator;

			Editor::Scene				scene;
			std::shared_ptr<reactphysics3d::PhysicsCommon>	physCom;
			Physics::PhysicsSimulator						physSim;

		private:
			void TryResizeWindow();

		public:
		/* CONSTRUCTORS/DESTRUCTORS */
		Engine();
		~Engine();


		void Run();

	};
}

#endif /* __ENGINE_HPP__ */
