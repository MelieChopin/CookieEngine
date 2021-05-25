#ifndef	__GAMESPECTOR_W_HPP__
#define __GAMESPECTOR_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int GamespectorID = 1 << 0; }

	class Gamespector : public GameWindowBase
	{
	public:
		inline Gamespector()
			: GameWindowBase("Gamespector", true)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::GamespectorID; };
	};
}

#endif