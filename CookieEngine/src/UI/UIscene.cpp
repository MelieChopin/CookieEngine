#include "UIscene.hpp"

#include "UIgame_AllIn.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UI;
using namespace Cookie::UIwidget;


UIscene::UIscene(const std::vector<GameWindowInfo>& GameWindows)
{
	if (!GameWindows.empty())
		LoadLayout(GameWindows);
}


void UIscene::LoadLayout(const std::vector<GameWindowInfo>& GameWindows)
{
	for (const GameWindowInfo& info : GameWindows)
	{ 
		std::unique_ptr<GameWindowBase> convertedWidget;

		switch (info.ID)
		{
		case GameWidgetID::TestBoiID: convertedWidget = std::make_unique<TestBoi>(TestBoi()); break;

		default: break;
		}

		convertedWidget->xPos = info.xPos;
		convertedWidget->yPos = info.yPos;
		convertedWidget->width = info.width;
		convertedWidget->height = info.height;


		sceneWidgets.push_back(convertedWidget);
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

	std::vector<std::unique_ptr<UIwidget::GameWindowBase>>().swap(sceneWidgets);
}