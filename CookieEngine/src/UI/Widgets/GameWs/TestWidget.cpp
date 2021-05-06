#include "TestWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


void TestBoi::WindowDisplay()
{
	TryBeginWindow()
	{
		Text("OOooohohohoho!");
	}

	ImGui::EndChild();
}