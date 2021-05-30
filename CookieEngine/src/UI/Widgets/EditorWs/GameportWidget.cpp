#include "Game.hpp"
#include "GameportWidget.hpp"

#include "Scene.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


bool GamePort::BeginWindow(int windowFlags)
{
	if (isPlaying)
	{	
		contentVisible = ImGui::Begin(windowName, (isPlaying ? nullptr : &opened), windowFlags);

		return true;
	}
	else return false;
}


void GamePort::WindowDisplay()
{
	TryBeginWindow(ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus)
	{
		if (!isPlaying)
		{ Text("[Game not running]"); }

		else
		{
			ImGui::Image(static_cast<ImTextureID>(game.frameBuffer.shaderResource), { (float)game.renderer.window.width, (float)game.renderer.window.height });
			
			game.scene->uiScene.RenderLayout();
		}
	}

	ImGui::End();
}