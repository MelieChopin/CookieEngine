#include "EntityHandler.hpp"
#include "GamespectorWidget.hpp"

#include <imgui.h>

using namespace Cookie::UIwidget;
using namespace Cookie::ECS;
using namespace ImGui;


void Gamespector::WindowDisplay()
{
	TryBeginWindow()
	{
		if (selectedEntities.size() == 1)
		{
			const Entity* const & selectedEntity = selectedEntities[0];

		}
	}

	ImGui::End();
}