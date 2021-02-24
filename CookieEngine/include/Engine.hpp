#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Render/Renderer.hpp"
#include "Core/Window.hpp"
#include "Input.hpp"
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
			Core::Input 				input;
			Render::Renderer			renderer;
			Core::UIcore				ui;
			Resources::ResourcesManager	resources;
			std::shared_ptr<Render::Camera>	camera = nullptr;
			Render::FrameBuffer			frameBuffer;
			ECS::Coordinator			coordinator;
			std::vector<Editor::Scene>	scene;
			int							indexScene = 0;

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
