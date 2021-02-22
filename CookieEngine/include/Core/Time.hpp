#ifndef __TIME_HPP__
#define __TIME_HPP__


#include <GLFW/glfw3.h>


namespace Cookie
{
	namespace Core
	{
		/*
		class Time
		{
		public:
			static float timeScale;
			static float deltaTime;
			static float unscaledDeltaTime;
			static float lastFrame;

			Time() {}
			~Time() {}

			static void UpdateTime();
		};*/

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
/*
float Cookie::Core::Time::timeScale = 1.f;
float Cookie::Core::Time::deltaTime = 0.f;
float Cookie::Core::Time::unscaledDeltaTime = 0.f;
float Cookie::Core::Time::lastFrame = 0.f;

void Cookie::Core::Time::UpdateTime()
{
	float currentFrame = glfwGetTime();

	unscaledDeltaTime = (currentFrame - lastFrame);
	deltaTime = unscaledDeltaTime * timeScale;
	lastFrame = currentFrame;
}*/



#endif // !__TIME__
