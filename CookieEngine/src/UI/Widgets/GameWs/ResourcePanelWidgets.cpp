#include "Income.hpp"
#include "ResourcePanelWidgets.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Gameplay;


void WheatPanel::WindowDisplay()
{
	TryBeginWindow()
	{
		TextColored({ 1.f, 0.816f, 0.31f, 1.f }, "Wheat: %.1f", income.primary);
	}

	ImGui::End();
}

void ChocolatePanel::WindowDisplay()
{
	TryBeginWindow()
	{
		TextColored({ 1.f, 0.816f, 0.31f, 1.f }, "Chocolate: %.1f", income.secondary);
	}

	ImGui::End();
}