#include "UIscene.hpp"

#include "UIgame_AllIn.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UI;
using namespace Cookie::UIwidget;


UIscene::UIscene(const std::vector<GameWindowInfo>& _gameWindows)
{
	if (!_gameWindows.empty())
		LoadLayout(_gameWindows);
}

UIscene::~UIscene()
{ CleanLayout(); }


void UIscene::LoadLayout(const std::vector<GameWindowInfo>& GameWindows)
{
	for (const GameWindowInfo& info : GameWindows)
	{ 
		GameWindowBase* convertedWidget = nullptr;

		switch (info.ID)
		{
		case GameWidgetID::TestBoiID: convertedWidget = new TestBoi; break;

		default: break;
		}

		convertedWidget->xPos = info.xPos;
		convertedWidget->yPos = info.yPos;
		convertedWidget->width = info.width;
		convertedWidget->height = info.height;


		sceneWidgets.push_back(std::move(convertedWidget));
	}
}

const std::vector<UIscene::GameWindowInfo> UIscene::SaveLayout(bool clean)
{
	std::vector<GameWindowInfo> infos;
	
	for (GameWindowBase*& widget : sceneWidgets)
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
	for (GameWindowBase*& widget : sceneWidgets)
	{ delete widget; }

	std::vector<GameWindowBase*>().swap(sceneWidgets);
}