#include "UI/UIwidgetBases.h"

using namespace ImGui;
using namespace Cookie::UIwidget;

WindowBase::WindowBase(const char* _windowName, bool _opened)
			: windowName	(_windowName),
			  opened		(_opened)
{}


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


void WItemBase::ItemDisplay()
{
	if (visible)
	{
		if (MenuItem(itemName, shortcutSeq)) Flip();
	}
}