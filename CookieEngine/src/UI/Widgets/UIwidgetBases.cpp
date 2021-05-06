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
	if (!opened || invalid) return false;

	SetNextWindowPos ({ xPos + GetWindowPos().x, yPos + GetWindowPos().y }, ImGuiCond_Appearing);
	SetNextWindowSize({ width				   , height					 }, ImGuiCond_Appearing);

	SetNextWindowViewport(GetWindowViewport()->ID);
	contentVisible = Begin(windowName, nullptr, windowFlags | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking);

	return true;
}

bool GameWindowBase::WindowEdit()
{
	const ImVec2&& parentWindowPos  = GetWindowPos();
	const ImVec2&& parentWindowSize = GetWindowSize();

	SetNextWindowSize({ width, height }, ImGuiCond_Appearing);


	invalid = (xPos < 0) || (yPos < 0) || (xPos + width > parentWindowSize.x) || (yPos + height > parentWindowSize.y);

	if (invalid)
	{
		PushStyleColor(ImGuiCol_TitleBg			, { 0.75, 0, 0, 1 });
		PushStyleColor(ImGuiCol_TitleBgActive	, { 1   , 0, 0, 1 });
	}

	SetNextWindowViewport(GetWindowViewport()->ID);
	ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

	SetWindowPos({ xPos + parentWindowPos.x, yPos + parentWindowPos.y }, IsWindowFocused() ? ImGuiCond_Appearing : ImGuiCond_Always);

	if (IsWindowFocused())
	{
		xPos	= ((int)((GetWindowPos().x - parentWindowPos.x)	/ 10)) * 10;
		yPos	= ((int)((GetWindowPos().y - parentWindowPos.y)	/ 10)) * 10;
		width	= ((int)(GetWindowWidth()	/ 10)) * 10;
		height	= ((int)(GetWindowHeight()	/ 10)) * 10;

		SetWindowPos ({ xPos + parentWindowPos.x, yPos + parentWindowPos.y  });
		SetWindowSize({ width					, height					});

		if (!invalid)
		{
			if (Button("Delete this widget"))
			{
				ImGui::End();
				return false;
			}
		}
		else if (IsWindowHovered())
		{
			BeginTooltip();
			TextDisabled("This widget is currently illegally placed.\nIt prevents saving, can't be deleted, and will not appear in a game run.\nMove it back inside the editor.");
			EndTooltip();
		}
	}

	ImGui::End();
	
	if (invalid) PopStyleColor(2);
	

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