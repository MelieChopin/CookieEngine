#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__


#include "Core/Window.hpp"
#include "Input.hpp"

namespace Cookie
{
	class Engine
	{
		public:
			Core::Window window;
			Core::Input input;

		public:
		/* CONSTRUCTORS/DESTRUCTORS */
		Engine();
		~Engine();


		void Run();

	};
}

#endif /* __ENGINE_HPP__ */