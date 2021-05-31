#ifndef	__GAMESPECTOR_W_HPP__
#define __GAMESPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

#include <vector>

namespace Cookie::ECS { class Coordinator; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int GamespectorID = 1 << 0; }

	class Gamespector final : public GameWindowBase
	{
		const Cookie::ECS::Coordinator& coordinator;

	public:
		inline Gamespector(const Cookie::ECS::Coordinator& _coordinator)
			: GameWindowBase	("Gamespector", true),
			  coordinator		(_coordinator)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::GamespectorID; };
	};
}

#endif