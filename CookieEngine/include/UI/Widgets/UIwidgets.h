#ifndef	__COOKIEUI_WIDGETS_HPP__
#define __COOKIEUI_WIDGETS_HPP__

#include "UIwidgetBases.h"

struct GLFWwindow;


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



	class ExitPannel final : public WItemBase
	{
		GLFWwindow* window;

	public:
		ExitPannel(GLFWwindow* _window)
			: WItemBase	("Exit confirmation", "Exit", "Esc"),
			  window	(_window)
		{}

		void WindowDisplay() override;
	};
}

#endif