#ifndef __TIME_HPP__
#define __TIME_HPP__


#include <GLFW/glfw3.h>


namespace Cookie
{
	namespace Core
	{
		static float timeScale {1.f};
		static float deltaTime {0.f};
		static float unscaledDeltaTime {0.f};
		static float lastFrame {0.f};

		static void UpdateTime() 
		{
			float currentFrame = glfwGetTime();

			unscaledDeltaTime = (currentFrame - lastFrame);
			deltaTime = unscaledDeltaTime * timeScale;
			lastFrame = currentFrame;
		}
	}
}



#endif // !__TIME_HPP__
