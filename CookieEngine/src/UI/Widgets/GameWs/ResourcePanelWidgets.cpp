#include "Income.hpp"
#include "ResourcePanelWidgets.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Gameplay;


void IncomePanel::WindowDisplay()
{
	TryBeginWindow()
	{
		TextColored({ 1.f,    0.816f, 0.31f,  1.f }, "Wheat:%.1f", income.primary);
		TextColored({ 0.482f, 0.247f, 0.f,    1.f }, "Chocolate:%.1f", income.secondary);
		TextColored({ 0.454f, 0.329f, 0.204f, 1.f }, "Population:%.0f", income.supplyCurrent);
	}

	ImGui::End();
}