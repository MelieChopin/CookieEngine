#ifndef __CONSOLE_W_HPP__
#define __CONSOLE_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Core 
{
	class  DebugMessageHandler;
	struct DebugMessage;
}

namespace Cookie::Resources { class  Texture;	}
namespace Cookie::Render	{ class  Renderer;	}


namespace Cookie::UIwidget
{
	class Console final : public WItemBase
	{
		Cookie::Core::DebugMessageHandler& debugManager;
		
		uint8_t errorFlash = 0;

		bool messagesGrouped = false;

		std::unique_ptr<Cookie::Resources::Texture> icons[3];

	private:
		void UngroupedDisplay();
		void GroupedDisplay();

		void DisplayMessage(Cookie::Core::DebugMessage& message);
		void MessageColorBounce(unsigned short intensity, uint8_t& colorVariant, bool& bouncing, unsigned short& colorBounces);

	protected:
		bool BeginWindow(int windowFlags = 0) final override;

	public:
		Console(Cookie::Core::DebugMessageHandler& _debugManager, Cookie::Render::Renderer& _renderer);
		Console(const Console&) = delete;
		Console(Console&&)		= default;

		void WindowDisplay() override;
	};
}

#endif