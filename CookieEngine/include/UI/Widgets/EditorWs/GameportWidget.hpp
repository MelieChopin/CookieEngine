#ifndef __GAMEPORT_W_HPP__
#define __GAMEPORT_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie { class Game; }


namespace Cookie::UIwidget
{
	class GamePort final : public WItemBase
	{
		const Cookie::Game& game;

	protected:
		bool BeginWindow(int windowFlags = 0) override;
	
	public:
		GamePort(const Cookie::Game& _game)
			: WItemBase	("Gameport"),
			  game		(_game)
		{}

		void WindowDisplay() override;
	};
}

#endif