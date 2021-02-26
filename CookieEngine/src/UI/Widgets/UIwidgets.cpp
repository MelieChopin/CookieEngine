#include "Coordinator.hpp"
#include "FrameBuffer.hpp"
#include <GLFW/glfw3.h>
#include "UIwidgets.h"

#include <imgui.h>


using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;


void GamePort::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);
    ImGui::End();
}


void ExitPannel::WindowDisplay()
{
	if (!BeginWindow(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)) return;

	TextColored({ 1, 0, 0, 1 }, "Are you sure you want to close Cookie engine?");

	if (Button("Yes, close!", { 150, 50 })) glfwSetWindowShouldClose(window, true);
	SameLine(160.f);
	if (Button("Nooo, stay!", { 150, 50 })) Flip();

	ImGui::End();
}

