
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