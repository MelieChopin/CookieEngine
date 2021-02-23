#include "Coordinator.hpp"
#include "FrameBuffer.hpp"
#include <GLFW/glfw3.h>
#include "UIwidgets.h"

#include <imgui.h>


using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;


void Viewport::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);

	ImGui::Image(static_cast<ImTextureID>(*frameBuffer.GetShaderResource()), GetContentRegionAvail());

	if (IsWindowHovered() && GetIO().MouseClicked[0])
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseCapture = true;
	}

    ImGui::End();
}


void GamePort::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);
    ImGui::End();
}


void ExitPannel::WindowDisplay()
{
	if (!opened) return;


	ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

	TextColored({ 1, 0, 0, 1 }, "Are you sure you want to close Cookie engine?");

	if (Button("Yes, close!", { 150, 50 })) glfwSetWindowShouldClose(window, true);
	SameLine(160.f);
	if (Button("Nooo, stay!", { 150, 50 })) Flip();

	ImGui::End();
}

