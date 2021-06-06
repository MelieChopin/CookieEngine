#ifndef __GAME_UI_EDITOR_W_HPP__
#define __GAME_UI_EDITOR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie { class Game; }


namespace Cookie::UIwidget
{
	class GameUIeditor final : public WItemBase
	{
		Cookie::Game& game;

	public:
		inline GameUIeditor(Cookie::Game& _game)
			: WItemBase	("UI editor"),
			  game		(_game)
		{}

		void WindowDisplay() override;
	};
}

#endif