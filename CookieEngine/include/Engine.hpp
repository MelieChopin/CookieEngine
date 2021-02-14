#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__


#include "Core/Window.hpp"

namespace Cookie
{
	class Engine
	{
		public:
			Core::Window window;

		public:
		/* CONSTRUCTORS/DESTRUCTORS */
		Engine();
		~Engine();


		void Run();

	};
}

#endif /* __ENGINE_HPP__ */