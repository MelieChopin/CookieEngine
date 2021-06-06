#ifndef __GAMEPORT_W_HPP__
#define __GAMEPORT_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie { class Game; }


namespace Cookie::UIwidget
{
	class GamePort final : public WindowBase
	{
		const bool& isPlaying;
		const Cookie::Game& game;

	protected:
		bool BeginWindow(int windowFlags = 0) override;
	
	public:
		GamePort(bool& _isPlaying, const Cookie::Game& _game)
			: WindowBase("Gameport", false),
			  isPlaying (_isPlaying),
			  game		(_game)
		{}

		void WindowDisplay() override;
	};
}

#endif