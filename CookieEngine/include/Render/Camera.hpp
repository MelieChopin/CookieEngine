#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Core/Math/Mat4.hpp"
#include "Core/Math/Calc.hpp"

#include "Core/Time.hpp"

namespace Cookie
{
	namespace Render
	{
		#define CAM_MOUSE_SPEED 50.f
		#define CAM_MOUSE_SPEED_UP_SCALE 10.f
		#define CAM_MOUSE_SENSITIVITY_X 0.0007f
		#define CAM_MOUSE_SENSITIVITY_Y 0.0007f

		#define CAMERA_INITIAL_NEAR 0.01f
		#define CAMERA_INITIAL_FAR  10000.f

		class Camera
		{
			private:
				Core::Math::Mat4 projMat;
				Core::Math::Mat4 viewMat;
				double previousMouseX {0.0};
				double previousMouseY {0.0};

			public:
				Core::Math::Vec3 pos = {0.0f,0.0f,0.0f};
				Core::Math::Vec3 rot = {0.0f,0.0f,0.0f};

			private:

			public:
				Camera() {}
				~Camera() {}

				inline Core::Math::Mat4 GetViewProj() const;
				inline void SetProj(float yFov, float aspect, float n, float f);
				
				inline void UpdateFreeFly(GLFWwindow* window);
				inline void UpdateFreeFlyPos(GLFWwindow* window);
				inline void UpdateFreeFlyRot(GLFWwindow* window);
				inline void Update();

				inline void ResetPreviousMousePos(GLFWwindow* window);
		};
	}
}

#include "Camera.inl"

#endif // __CAMERA_HPP__
