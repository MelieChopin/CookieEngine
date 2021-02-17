#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "Render/Renderer.hpp"
#include "Core/Window.hpp"
#include "Input.hpp"
#include "Coordinator.hpp"

namespace Cookie
{
	class Engine
	{
		public:
			Core::Window		window;
			Core::Input 		input;
			Render::Renderer	renderer;
			ECS::Coordinator    coordinator;
			

		public:
		/* CONSTRUCTORS/DESTRUCTORS */
		Engine();
		~Engine();


		void Run();

	};
}

#endif /* __ENGINE_HPP__ */