#include "Scene.hpp"
#include "Game.hpp"
#include "GameUIeditorWidget.hpp"

#include "UIgame_AllIn.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


void GameUIeditor::WindowDisplay()
{
	TryBeginWindow(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBringToFrontOnFocus)
	{
		if (BeginChild("Widget pannel", { 200.f, (float)game.renderer.window.height}, true))
		{
			if (Selectable("Add a test"))
				game.scene->uiScene.GetLayout().push_back(new TestBoi());

			if (Selectable("Add a gamespector"))
				game.scene->uiScene.GetLayout().push_back(new Gamespector(game.coordinator, game.resources.icons["Assets/EditorUIcons/Warning.ico"].get()));
		}
		
		EndChild();


		SameLine();

		if (BeginChild("Widget space", { (float)game.renderer.window.width, (float)game.renderer.window.height }, true, ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			std::vector<GameWindowBase*>& widgets = game.scene->uiScene.GetLayout();

			for (auto wit = widgets.begin(); wit != widgets.end();)
			{
				if ((*wit)->WindowEdit())
					wit++;
				else
					wit = widgets.erase(wit);
			}
		}
		
		EndChild();
	}

	ImGui::End();
}