#ifndef __GAME_UI_EDITOR_W_HPP__
#define __GAME_UI_EDITOR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Core		{ class Window; }
namespace Cookie::Resources { class Scene; }


namespace Cookie::UIwidget
{
	class GameUIeditor final : public WItemBase
	{
		const Cookie::Core::Window& window;
		std::shared_ptr<Cookie::Resources::Scene>& scene;

	public:
		inline GameUIeditor(const Cookie::Core::Window& _window, std::shared_ptr<Cookie::Resources::Scene>& _scene)
			: WItemBase	("UI editor"),
			  window	(_window),
			  scene		(_scene)
		{}

		void WindowDisplay() override;
	};
}

#endif