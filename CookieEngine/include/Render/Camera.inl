#ifndef __CAMERA_INL__
#define __CAMERA_INL__

#include <imgui.h>
#include <algorithm>

namespace Cookie
{
	namespace Render
	{
		inline Core::Math::Mat4 Camera::GetViewProj() const
		{ 
			return projMat * viewMat;
		}
		inline void Camera::SetProj(float yFov, float aspect, float n, float f)
		{
			projMat = Core::Math::Mat4::Perspective(yFov, aspect, n, f);
		}

		inline void Camera::UpdateFreeFly(GLFWwindow* window)
		{
			UpdateFreeFlyPos(window);
			UpdateFreeFlyRot(window);
			Update();
		}
		inline void Camera::UpdateFreeFlyPos(GLFWwindow* window)
		{
			// Spheric coordinates
			float cosPitch = std::cos(rot.x);
			float sinPitch = std::sin(rot.x);
			float cosYaw = std::cos(rot.y);
			float sinYaw = std::sin(rot.y);

			// Compute speed
			float speed = (CAM_MOUSE_SPEED * ImGui::GetIO().DeltaTime);
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
				speed *= CAM_MOUSE_SPEED_UP_SCALE;

			// Move forward/backward
			float forwardVelocity = 0.f;
			if (glfwGetKey(window, GLFW_KEY_W)) forwardVelocity -= speed;
			else if (glfwGetKey(window, GLFW_KEY_S)) forwardVelocity += speed;

			// Strafe left/right
			float strafeVelocity = 0.f;
			if (glfwGetKey(window, GLFW_KEY_A))  strafeVelocity -= speed;
			else if (glfwGetKey(window, GLFW_KEY_D)) strafeVelocity += speed;

			// Forward movement
			pos.z += cosYaw * cosPitch * forwardVelocity;
			pos.x += -sinYaw * cosPitch * forwardVelocity;
			pos.y += sinPitch * forwardVelocity;

			// Strafe movement
			pos.z += sinYaw * strafeVelocity;
			pos.x += cosYaw * strafeVelocity;

			// Up movement
			if (glfwGetKey(window, GLFW_KEY_SPACE))   pos.y += speed;
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) pos.y -= speed;
		}
		inline void Camera::UpdateFreeFlyRot(GLFWwindow* window)
		{
			//Calculate DeltaMousePos, later on will put in inputs
			double tempMouseX;
			double tempMouseY;
			glfwGetCursorPos(window, &tempMouseX, &tempMouseY);

			double deltaMouseX = tempMouseX - previousMouseX;
			double deltaMouseY = tempMouseY - previousMouseY;
			previousMouseX = tempMouseX;
			previousMouseY = tempMouseY;

			// Pitch
			rot.x += deltaMouseY * CAM_MOUSE_SENSITIVITY_Y;
			rot.x = std::clamp(rot.x, -Core::Math::TAU / 4.f, Core::Math::TAU / 4.f); // Limit rotation to -90,90 range

			// Yaw
			rot.y += deltaMouseX * CAM_MOUSE_SENSITIVITY_X;
			rot.y = std::fmod(rot.y + Core::Math::TAU + Core::Math::PI, Core::Math::TAU) - Core::Math::PI; // Loop around -180,180
		}

		inline void Camera::Update()
		{
			viewMat = Core::Math::Mat4::RotateX(-rot.x) * Core::Math::Mat4::RotateY(-rot.y) * Core::Math::Mat4::Translate(-pos);
		}

		inline void Camera::ResetPreviousMousePos(GLFWwindow* window)
		{
			double tempMouseX;
			double tempMouseY;
			glfwGetCursorPos(window, &tempMouseX, &tempMouseY);

			previousMouseX = tempMouseX;
			previousMouseY = tempMouseY;
		}
	}
}

#endif