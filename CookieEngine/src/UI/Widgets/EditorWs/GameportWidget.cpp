#include "Game.hpp"
#include "GameportWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;

bool gameruntest = false;


bool GamePort::BeginWindow(int windowFlags)
{
	if (!opened) 
		if (!gameruntest)	return false;
		else				Flip();

	if (gameruntest)
	{
		windowFlags |= ImGuiWindowFlags_NoDocking;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		windowFlags |= ImGuiWindowFlags_NoDecoration;
	}
	
	contentVisible = ImGui::Begin(windowName, (gameruntest ? nullptr : &opened), windowFlags);

	if (!opened) visible = true;

	return true;
}


void GamePort::WindowDisplay()
{
	if (IsKeyPressed(GLFW_KEY_O))
		gameruntest ^= 1;


	TryBeginWindow()
	{
		if (!gameruntest)
		{ Text("[Game not running]"); }

		else
		{ Text("[HA]"); }
	}

	ImGui::End();
}