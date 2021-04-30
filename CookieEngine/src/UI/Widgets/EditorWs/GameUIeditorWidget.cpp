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
		if (BeginChild("Widget pannel", { 60.f, (float)window.height}, true))
		{
			if (Selectable("Add a test!"))
				scene->uiScene.GetWidgets().push_back(new TestBoi());
		}
		
		EndChild();


		SameLine();

		if (BeginChild("Widget space", { (float)window.width, (float)window.height }, true))
		{
			std::vector<GameWindowBase*>& widgets = scene->uiScene.GetWidgets();

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