#ifndef	__GAMESPECTOR_W_HPP__
#define __GAMESPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::ECS		{ class Coordinator; }
namespace Cookie::Resources { class ResourcesManager; class Texture; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int GamespectorID = 1 << 0; }

	class Gamespector final : public GameWindowBase
	{
		const Cookie::ECS::Coordinator& coordinator;

		Cookie::Resources::ResourcesManager& resources;

	private:
		void SafeIcon(const Cookie::Resources::Texture* const & texture, const float size);

		void LifeBar(const float currentLife, const float maxLife, const float barWidth, const float barHeight);
		
	public:
		inline Gamespector(const Cookie::ECS::Coordinator& _coordinator, Cookie::Resources::ResourcesManager& _resources)
			: GameWindowBase	("Gamespector", true),
			  coordinator		(_coordinator),
			  resources			(_resources)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::GamespectorID; };
	};
}

#endif