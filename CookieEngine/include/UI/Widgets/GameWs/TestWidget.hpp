#ifndef __TEST_W_HPP__
#define	__TEST_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	class TestBoi final : public GameWindowBase
	{
	public:
		inline TestBoi()
			: GameWindowBase("Professional tester!", true)
		{}

		void WindowDisplay() override;
	};
}

#endif