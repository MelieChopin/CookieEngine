#ifndef	__COOKIEUI_WIDGETS_HPP__
#define __COOKIEUI_WIDGETS_HPP__


#include "UIwidgetBases.h"


struct GLFWwindow;

namespace Cookie::Render {class FrameBuffer;}


namespace Cookie::UIwidget
{
	class Viewport final : public WindowBase
	{
		GLFWwindow* window;
		const Cookie::Render::FrameBuffer& frameBuffer;
		bool& mouseCapture;

	public:
		Viewport(GLFWwindow* _window, const Cookie::Render::FrameBuffer& _frameBuffer, bool& _mouseCapture)
			: WindowBase	("Viewport"),
			  window		(_window),
			  frameBuffer	(_frameBuffer),
			  mouseCapture	(_mouseCapture)
		{}

		void WindowDisplay() override;
	};
	

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