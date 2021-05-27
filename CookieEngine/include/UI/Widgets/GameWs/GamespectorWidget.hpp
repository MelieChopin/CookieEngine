#ifndef	__GAMESPECTOR_W_HPP__
#define __GAMESPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

#include <vector>

namespace Cookie::ECS { class Entity; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int GamespectorID = 1 << 0; }

	class Gamespector final : public GameWindowBase
	{
		const std::vector<Cookie::ECS::Entity*>& selectedEntities;

	public:
		inline Gamespector(const std::vector<Cookie::ECS::Entity*>& _selectedEntities)
			: GameWindowBase	("Gamespector", true),
			  selectedEntities	(_selectedEntities)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::GamespectorID; };
	};
}

#endif