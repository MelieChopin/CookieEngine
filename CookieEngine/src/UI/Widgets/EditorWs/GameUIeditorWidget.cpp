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
		static std::vector<std::unique_ptr<GameWindowBase>>& widgets = game.scene->uiScene.GetLayout();
		
		if (BeginChild("Widget pannel", { 200.f, (float)game.renderer.window.height}, true))
		{
			if (game.scene->uiScene.signature & GameWidgetID::GamespectorID)
			{
				TextDisabled("Game inspector added");
			}
			else if (Selectable("Add a game inspector"))
			{
				widgets.push_back(std::make_unique<Gamespector>(game.coordinator, game.resources));
				game.scene->uiScene.signature |= GameWidgetID::GamespectorID;
			}


			if (game.scene->uiScene.signature & GameWidgetID::ActionPanelID)
			{
				TextDisabled("Action panel added");
			}
			else if (Selectable("Add an action panel"))
			{
				widgets.push_back(std::make_unique<ActionPanel>(game.coordinator, game.resources));
				game.scene->uiScene.signature |= GameWidgetID::ActionPanelID;
			}
		}
		
		EndChild();


		SameLine();

		if (BeginChild("Widget space", { (float)game.renderer.window.width, (float)game.renderer.window.height }, true, ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			for (size_t i = 0; i < widgets.size();)
			{
				if (widgets[i]->WindowEdit())
					i++;
				else
				{ 
					game.scene->uiScene.signature -= widgets[i]->GetID();
					widgets.erase(widgets.begin() + i);
				}
			}
		}
		
		EndChild();
	}

	ImGui::End();
}