#include "Window.hpp"
#include "Scene.hpp"
#include "GameUIeditorWidget.hpp"

#include "TestWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


void GameUIeditor::WindowDisplay()
{
	TryBeginWindow(ImGuiWindowFlags_AlwaysAutoResize)
	{
		if (BeginChild("Widget pannel", { 200.f, (float)window.height}, true))
		{
			if (Selectable("Add a test!"))
				scene->uiScene.GetLayout().push_back(new TestBoi());
		}
		
		EndChild();


		SameLine();

		if (BeginChild("Widget space", { (float)window.width, (float)window.height }, true, ImGuiWindowFlags_NoBringToFrontOnFocus))
		{
			std::vector<GameWindowBase*>& widgets = scene->uiScene.GetLayout();

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