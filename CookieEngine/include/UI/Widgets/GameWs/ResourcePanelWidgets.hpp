#ifndef __RESOURCE_PANEL_WS_HPP__
#define __RESOURCE_PANEL_WS_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Gameplay { struct Income; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int IncomePanelID = 1 << 3; }

	class IncomePanel final : public GameWindowBase
	{
		const Cookie::Gameplay::Income& income;

	public:
		inline IncomePanel(const Cookie::Gameplay::Income& _income)
			: GameWindowBase("Wheat panel", true),
			  income		(_income)
		{}
	
		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::IncomePanelID; };
	};
}



#endif // !__RESOURCE_PANEL_WS_HPP__
