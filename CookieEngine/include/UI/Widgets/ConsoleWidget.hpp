#ifndef __CONSOLE_W_HPP__
#define __CONSOLE_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Core 
{
	class  DebugMessageHandler;
	struct DebugMessage;
}


namespace Cookie::UIwidget
{
	class Console final : public WItemBase
	{
		Cookie::Core::DebugMessageHandler& debugManager;

		bool messagesGrouped = false;

	private:
		void UngroupedDisplay();
		void GroupedDisplay();

		void DisplayMessage(Cookie::Core::DebugMessage& message);
		void MessageColorBounce(unsigned short intensity, uint8_t& colorVariant, bool& bouncing, unsigned short& colorBounces);

	public:
		Console(Cookie::Core::DebugMessageHandler& _debugManager)
			: WItemBase		("Console", false),
			  debugManager	(_debugManager)
		{}

		void WindowDisplay() override;
	};
}

#endif