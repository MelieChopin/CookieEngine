#include "Game.hpp"
#include "GameportWidget.hpp"
#include "Scene.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


bool GamePort::BeginWindow(int windowFlags)
{
	if (!opened)
	{
		if (!isPlaying)	return false;
		else			Flip();
	}

	if (isPlaying)
	{
		windowFlags |= ImGuiWindowFlags_NoDocking;
		windowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
		windowFlags |= ImGuiWindowFlags_NoDecoration;
	}
	
	contentVisible = ImGui::Begin(windowName, (isPlaying ? nullptr : &opened), windowFlags);

	if (!opened) visible = true;

	return true;
}


void GamePort::WindowDisplay()
{
	TryBeginWindow()
	{
		if (!isPlaying)
		{ 
			game.scene->camera->Deactivate();
			Text("[Game not running]"); 
		}
		else
		{
			ImVec2 gameportPos;

			gameportPos.x = GetWindowPos().x + GetCursorPosX();
			gameportPos.y = GetWindowPos().y + GetCursorPosY();

			game.scene->camera->windowOffset = { {gameportPos.x, gameportPos.y} };
			game.scene->camera->Activate();

			ImGui::Image(static_cast<ImTextureID>(game.frameBuffer.shaderResource), {(float)game.renderer.window.width, (float)game.renderer.window.height});
		}
	}

	ImGui::End();
}