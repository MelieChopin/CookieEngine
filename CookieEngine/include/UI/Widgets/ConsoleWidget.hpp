#ifndef __CONSOLE_W_HPP__
#define __CONSOLE_W_HPP__

#include "UIwidgetBases.h"


namespace Cookie::Core {class Debug;}


namespace Cookie::UIwidget
{
	class Console final : public WindowBase
	{
		Cookie::Core::Debug& debugManager;

	private:
		void MessageColorBounce(unsigned short intensity, uint8_t& colorVariant, bool& bouncing, unsigned short& colorBounces);

	public:
		Console(Cookie::Core::Debug& _debugManager)
			: WindowBase	("Console"),
			  debugManager	(_debugManager)
		{}

		void WindowDisplay() override;
	};
}

#endif