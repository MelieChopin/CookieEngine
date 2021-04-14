#ifndef __GAMEPORT_W_HPP__
#define __GAMEPORT_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	class GamePort final : public WindowBase
	{
	public:
		GamePort()
			: WindowBase("Game")
		{}

		void WindowDisplay() override;
	};
}

#endif