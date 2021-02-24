#include "FrameBuffer.hpp"
#include "Camera.hpp"
#include <imgui.h>
#include <GLFW/glfw3.h>
#include "ViewportWidget.hpp"


using namespace ImGui;
using namespace Cookie::UIwidget;


void Viewport::WindowDisplay()
{
	ImGui::Begin(windowName, nullptr);

	ImGui::Image(static_cast<ImTextureID>(*frameBuffer.GetShaderResource()), GetContentRegionAvail());

	if ((recordedWinSize.x != GetWindowSize().x) || (recordedWinSize.y != GetWindowSize().y))
	{
		recordedWinSize = {GetWindowSize().x, GetWindowSize().y};
		(*camera)->SetProj(Core::Math::ToRadians(60.f), recordedWinSize.x / recordedWinSize.y, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
	}

	ImGuiIO& io = GetIO();

	if (io.MouseDown[ImGuiMouseButton_Right] && IsWindowHovered())
	{
		if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			io.WantCaptureKeyboard = false;
			(*camera)->Activate();
			(*camera)->ResetPreviousMousePos();
		}
	}
	else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED && !io.MouseDown[ImGuiMouseButton_Right])
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		io.WantCaptureKeyboard = true;
		(*camera)->Deactivate();
	}

	ImGui::End();
}