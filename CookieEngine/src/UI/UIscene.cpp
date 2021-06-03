#include "Game.hpp"
#include "UIscene.hpp"

#include "UIgame_AllIn.hpp"

#include <imgui.h>
#include <imgui_internal.h>

using namespace ImGui;
using namespace Cookie::UI;
using namespace Cookie::UIwidget;


UIscene::UIscene(const std::vector<GameWindowInfo>& _gameWindows, Cookie::Game& _game)
{
	if (!_gameWindows.empty())
		LoadLayout(_gameWindows, _game);
}

UIscene::~UIscene()
{ CleanLayout(); }


void UIscene::LoadLayout(const std::vector<GameWindowInfo>& GameWindows, Cookie::Game& game)
{
	for (const GameWindowInfo& info : GameWindows)
	{ 
		switch (info.ID)
		{
		case GameWidgetID::GamespectorID:	sceneWidgets.push_back(std::make_unique<Gamespector>(game.coordinator, game.resources));
		case GameWidgetID::ActionPanelID:	sceneWidgets.push_back(std::make_unique<ActionPanel>(game.coordinator, game.resources));

		default: break;
		}

		GameWindowBase* const & convertedWidget = sceneWidgets.back().get();

		convertedWidget->xPos = info.xPos;
		convertedWidget->yPos = info.yPos;
		convertedWidget->width = info.width;
		convertedWidget->height = info.height;

		signature |= info.ID;
	}
}

const std::vector<UIscene::GameWindowInfo> UIscene::SaveLayout(bool clean)
{
	std::vector<GameWindowInfo> infos;
	
	for (std::unique_ptr<GameWindowBase>& widget : sceneWidgets)
	{
		GameWindowInfo widgetInfo;

		
		widgetInfo.ID = widget->GetID();

		widgetInfo.xPos = widget->xPos;
		widgetInfo.yPos = widget->yPos;
		widgetInfo.width = widget->width;
		widgetInfo.height = widget->height;


		infos.push_back(widgetInfo);
	}

	if (clean) CleanLayout();

	return infos;
}


void UIscene::CleanLayout()
{
	for (std::unique_ptr<GameWindowBase>& widget : sceneWidgets)
	{ widget.reset(); }

	std::vector<std::unique_ptr<GameWindowBase>>().swap(sceneWidgets);
}


void UIscene::RenderLayout()
{
	isHovered = false;
	const ImVec2 mPos = GetIO().MousePos;

	for (std::unique_ptr<GameWindowBase>& gw : sceneWidgets)
	{
		gw->WindowDisplay();

		const ImRect lastRect = FindWindowByName(gw->GetName())->Rect();

		isHovered |= (mPos.x > lastRect.Min.x) && (mPos.x < lastRect.Max.x) && (mPos.y > lastRect.Min.y) && (mPos.y < lastRect.Max.y);
	}
}