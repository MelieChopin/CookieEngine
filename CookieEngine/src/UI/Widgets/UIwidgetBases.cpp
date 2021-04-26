#include "UIwidgetBases.hpp"

#include <imgui.h>


using namespace ImGui;
using namespace Cookie::UIwidget;


WindowBase::WindowBase(const char* _windowName, bool _opened)
			: windowName	(_windowName),
			  opened		(_opened)
{}

bool WindowBase::BeginWindow(int windowFlags)
{
	if (!opened) return false;

	contentVisible = ImGui::Begin(windowName, nullptr, windowFlags);

	return true;
}


bool GameWindowBase::BeginWindow(int windowFlags)
{
	if (!opened) return false;

	SetNextWindowPos({xPos, yPos},		ImGuiCond_Appearing);
	SetNextWindowSize({width, height},	ImGuiCond_Appearing);

	contentVisible = ImGui::Begin(windowName, nullptr, windowFlags | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse);

	return true;
}

bool GameWindowBase::WindowEdit()
{
	BeginWindow();

	if (Button("Delete this widget"))
	{
		ImGui::End();
		return false;
	}

	if (IsWindowFocused())
	{
		xPos = GetWindowPos().x;
		yPos = GetWindowPos().y;
		width = GetWindowWidth();
		height = GetWindowHeight();
	}

	ImGui::End();
}


ItemBase::ItemBase(const char* _itemName, bool _visible)
		  :	itemName	(_itemName),
			shortcutSeq	(NULL),
			visible		(_visible)
{}

ItemBase::ItemBase(const char* _itemName, const char* _shortcutSeq, bool _visible)
		:	itemName	(_itemName),
			shortcutSeq	(_shortcutSeq),
			visible		(_visible)
{}

void ItemBase::ItemDisplay()
{
	if (visible) 
		MenuItem(itemName, shortcutSeq);
}


bool WItemBase::BeginWindow(int windowFlags)
{
	if (!opened) return false;

	contentVisible = ImGui::Begin(windowName, &opened, windowFlags);

	if (!opened) visible = true;
	
	return true;
}

void WItemBase::ItemDisplay()
{
	if (visible)
	{
		if (MenuItem(itemName, shortcutSeq)) Flip();
	}
	else
	{
		TextDisabled("%s", itemName);
	}
}