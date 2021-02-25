#ifndef __CAMERA_INL__
#define __CAMERA_INL__

#include <imgui.h>
#include <algorithm>
#include <Debug.hpp>
#include "Core/Math/Quat.hpp"

namespace Cookie
{
	namespace Render
	{
		/*======================= CAMERA =======================*/

		inline void Camera::Update()
		{
			viewMat = Core::Math::Mat4::Inverse(Core::Math::Mat4::Translate(pos) * Core::Math::Mat4::RotateY(rot.y)  * Core::Math::Mat4::RotateX(rot.x));
		}

		inline void Camera::ResetPreviousMousePos()
		{
			previousMouseX = ImGui::GetIO().MousePos.x;
			previousMouseY = ImGui::GetIO().MousePos.y;
		}					 

		inline Core::Math::Vec3 Camera::MouseToWorldDir()
		{
			Core::Math::Vec2 mousePos = { ImGui::GetIO().MousePos.x - windowOffset.x,ImGui::GetIO().MousePos.y - windowOffset.y};

			Core::Math::Vec2 ratio = { (mousePos.x / (width * 0.5f)) - 1.0f,  (-mousePos.y / (height * 0.5f)) + 1.0f };

			Core::Math::Vec4 r = Core::Math::Mat4::Inverse(projMat * viewMat) * Core::Math::Vec4(ratio.x,ratio.y,1.0f,1.0f);

			return Core::Math::Vec3({ r.x / r.a,r.y / r.a,r.z / r.a }).Normalize();
		}

		/*======================= FREE FLY CAMERA =======================*/


		inline void FreeFlyCam::Update()
		{
			if (activated)
			{
				UpdateFreeFlyPos();
				UpdateFreeFlyRot();
				Camera::Update();
			}
		}

		inline void FreeFlyCam::UpdateFreeFlyPos()
		{
			// Spheric coordinates
			float cosPitch = std::cos(rot.x);
			float sinPitch = std::sin(rot.x);
			float cosYaw = std::cos(rot.y);
			float sinYaw = std::sin(rot.y);

			// Compute speed
			float speed = (CAM_MOUSE_SPEED * ImGui::GetIO().DeltaTime);
			if (ImGui::GetIO().KeyShift)
				speed *= CAM_MOUSE_SPEED_UP_SCALE;

			// Move forward/backward
			float forwardVelocity = 0.f;
			if (ImGui::GetIO().KeysDown[GLFW_KEY_W]) forwardVelocity -= speed;
			else if (ImGui::GetIO().KeysDown[GLFW_KEY_S]) forwardVelocity += speed;

			// Strafe left/right
			float strafeVelocity = 0.f;
			if (ImGui::GetIO().KeysDown[GLFW_KEY_A])  strafeVelocity -= speed;
			else if (ImGui::GetIO().KeysDown[GLFW_KEY_D]) strafeVelocity += speed;

			// Forward movement
			pos.z += cosYaw * cosPitch * forwardVelocity;
			pos.x += -sinYaw * cosPitch * forwardVelocity;
			pos.y += sinPitch * forwardVelocity;

			// Strafe movement
			pos.z += sinYaw * strafeVelocity;
			pos.x += cosYaw * strafeVelocity;

			// Up movement
			if (ImGui::GetIO().KeysDown[GLFW_KEY_SPACE]) pos.y += speed;
			if (ImGui::GetIO().KeysDown[GLFW_KEY_LEFT_CONTROL]) pos.y -= speed;
		}

		inline void FreeFlyCam::UpdateFreeFlyRot()
		{
			//Calculate DeltaMousePos, later on will put in inputs
			float tempMouseX = ImGui::GetIO().MousePos.x;
			float tempMouseY = ImGui::GetIO().MousePos.y;

			float deltaMouseX = tempMouseX - previousMouseX;
			float deltaMouseY = tempMouseY - previousMouseY;

			previousMouseX = tempMouseX;
			previousMouseY = tempMouseY;

			// Pitch
			rot.x += deltaMouseY * CAM_MOUSE_SENSITIVITY_Y;
			rot.x = std::clamp(rot.x, -Core::Math::TAU / 4.f, Core::Math::TAU / 4.f); // Limit rotation to -90,90 range

			// Yaw
			rot.y += deltaMouseX * CAM_MOUSE_SENSITIVITY_X;
			rot.y = std::fmod(rot.y + Core::Math::TAU + Core::Math::PI, Core::Math::TAU) - Core::Math::PI; // Loop around -180,180
		}


		/* Game Camera*/

		inline void GameCam::UpdateGamePos()
		{
			if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left])
			{
				//Calculate DeltaMousePos, later on will put in inputs
				float tempMouseX = ImGui::GetIO().MousePos.x;
				float tempMouseY = ImGui::GetIO().MousePos.y;

				float deltaMouseX = (tempMouseX - previousMouseX) * CAM_MOUSE_SENSITIVITY_X;
				float deltaMouseY = (tempMouseY - previousMouseY) * CAM_MOUSE_SENSITIVITY_Y;

				previousMouseX = tempMouseX;
				previousMouseY = tempMouseY;

				float speed = (ImGui::GetIO().DeltaTime * CAM_MOUSE_SPEED * CAM_MOUSE_SPEED_UP_SCALE);

				pos -= Core::Math::Vec3(deltaMouseX, 0.0f, deltaMouseY) * speed;
			}
		}


		inline void GameCam::Update()
		{
			if (activated)
			{
				UpdateGamePos();
				Camera::Update();
			}

			ResetPreviousMousePos();
		}

	}
}

#endif
