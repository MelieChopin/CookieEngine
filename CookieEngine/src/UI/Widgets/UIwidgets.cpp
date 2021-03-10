#include "Coordinator.hpp"
#include "FrameBuffer.hpp"
#include <GLFW/glfw3.h>
#include "UIwidgets.hpp"

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
	TryBeginWindow(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking)
	{
		TextColored({ 1, 0, 0, 1 }, "Are you sure you want to close Cookie engine?");

		if (Button("Yes, close!", { 150, 50 })) glfwSetWindowShouldClose(window, true);
		SameLine(160.f);
		if (Button("Nooo, stay!", { 150, 50 })) Flip();
	}

	ImGui::End();
}

void DemoWindow::WindowDisplay()
{
	if (!opened) return;

	ShowDemoWindow(&opened);

	if (!opened) visible = true;
}