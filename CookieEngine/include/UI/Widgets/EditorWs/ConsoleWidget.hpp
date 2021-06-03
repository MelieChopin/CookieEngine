#ifndef __CONSOLE_W_HPP__
#define __CONSOLE_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Core		{ class DebugMessageHandler; struct DebugMessage; }
namespace Cookie::Resources { class ResourcesManager; class Texture; }


namespace Cookie::UIwidget
{
	class Console final : public WItemBase
	{
		Cookie::Core::DebugMessageHandler& debugManager;
		
		uint8_t errorFlash = 0;

		bool messagesGrouped = false;

		const Cookie::Resources::Texture* const icons[3];

	private:
		void UngroupedDisplay();
		void GroupedDisplay();

		void DisplayMessage(Cookie::Core::DebugMessage& message);
		void MessageColorBounce(unsigned short intensity, uint8_t& colorVariant, bool& bouncing, unsigned short& colorBounces);

	protected:
		bool BeginWindow(int windowFlags = 0) final override;

	public:
		Console(Cookie::Core::DebugMessageHandler& _debugManager, Cookie::Resources::ResourcesManager& _resources);
		Console(const Console&) = delete;
		Console(Console&&)		= default;

		void WindowDisplay() override;
	};
}

#endif