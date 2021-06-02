#ifndef __ACTION_PANEL_W_HPP__
#define __ACTION_PANEL_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::ECS		{ class Coordinator; }
namespace Cookie::Resources { class ResourcesManager; class Texture; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int ActionPanelID = 1 << 1; }

	class ActionPanel final : public GameWindowBase
	{
		const Cookie::ECS::Coordinator& coordinator;

		Cookie::Resources::ResourcesManager& resources;

	private:
		void SafeIcon(const Cookie::Resources::Texture* const & texture, const float size);

		void LifeBar(const float currentLife, const float maxLife, const float barWidth, const float barHeight);

	public:
		inline ActionPanel(const Cookie::ECS::Coordinator& _coordinator, Cookie::Resources::ResourcesManager& _resources)
			: GameWindowBase("Action panel", true),
			  coordinator	(_coordinator),
			  resources		(_resources)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::ActionPanelID; };
	};
}

#endif // !__ACTION_PANEL_W_HPP__
