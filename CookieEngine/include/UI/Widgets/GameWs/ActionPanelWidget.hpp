#ifndef __ACTION_PANEL_W_HPP__
#define __ACTION_PANEL_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int ActionPanelID = 1 << 1; }

	class ActionPanel final : public GameWindowBase
	{
	public:
		inline ActionPanel()
			: GameWindowBase("Action panel", true)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::ActionPanelID; };
	};
}

#endif // !__ACTION_PANEL_W_HPP__
