#ifndef __TEST_W_HPP__
#define	__TEST_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int TestBoiID = 1 << 20; }

	class TestBoi final : public GameWindowBase
	{
	public:
		inline TestBoi()
			: GameWindowBase("Professional tester!", true)
		{}

		void WindowDisplay() override;

		inline int GetID() override {return GameWidgetID::TestBoiID;};
	};
}

#endif