#ifndef __CONSOLE_W_HPP__
#define __CONSOLE_W_HPP__

#include "UIwidgetBases.h"

namespace Cookie::Core 
{
	class  Debug;
	struct DebugMessage;
}


namespace Cookie::UIwidget
{
	class Console final : public WItemBase
	{
		Cookie::Core::Debug& debugManager;

		bool messagesGrouped = false;

	private:
		void UngroupedDisplay();
		void GroupedDisplay();

		void DisplayMessage(Cookie::Core::DebugMessage& message);
		void MessageColorBounce(unsigned short intensity, uint8_t& colorVariant, bool& bouncing, unsigned short& colorBounces);

	public:
		Console(Cookie::Core::Debug& _debugManager)
			: WItemBase		("Console", false),
			  debugManager	(_debugManager)
		{}

		void WindowDisplay() override;
	};
}

#endif