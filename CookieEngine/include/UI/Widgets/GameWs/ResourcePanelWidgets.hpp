#ifndef __RESOURCE_PANEL_WS_HPP__
#define __RESOURCE_PANEL_WS_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Gameplay { struct Income; }



namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ 
		constexpr int WheatPanelID		= 1 << 3;
		constexpr int ChocolatePanelID	= 1 << 4;
	}

	class WheatPanel final : public GameWindowBase
	{
		const Cookie::Gameplay::Income& income;

	public:
		inline WheatPanel(const Cookie::Gameplay::Income& _income)
			: GameWindowBase("Wheat panel", true),
			  income		(_income)
		{}
	
		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::WheatPanelID; };
	};


	class ChocolatePanel final : public GameWindowBase
	{
		const Cookie::Gameplay::Income& income;

	public:
		inline ChocolatePanel(const Cookie::Gameplay::Income& _income)
			: GameWindowBase("Chocolate panel", true),
			  income		(_income)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::ChocolatePanelID; };
	};
}



#endif // !__RESOURCE_PANEL_WS_HPP__
