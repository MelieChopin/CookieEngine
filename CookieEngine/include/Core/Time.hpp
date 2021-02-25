#ifndef __TIME_HPP__
#define __TIME_HPP__

#include <ImGui/imgui.h>

namespace Cookie
{
	namespace Core
	{
		static float timeScale {1.f};
		static float deltaTime {0.f};
		static float unscaledDeltaTime {0.f};

		static inline void UpdateTime() 
		{
			unscaledDeltaTime = ImGui::GetIO().DeltaTime;
			deltaTime = unscaledDeltaTime * timeScale;
		}
	}
}



#endif // !__TIME_HPP__
