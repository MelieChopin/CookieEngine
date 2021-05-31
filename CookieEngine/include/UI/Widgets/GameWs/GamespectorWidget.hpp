#ifndef	__GAMESPECTOR_W_HPP__
#define __GAMESPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

#include <vector>

namespace Cookie::ECS { class Coordinator; }
namespace Cookie::Resources { class Texture; }

namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int GamespectorID = 1 << 0; }

	class Gamespector final : public GameWindowBase
	{
		const Cookie::ECS::Coordinator& coordinator;

		const Cookie::Resources::Texture* const & nullTexture;

	public:
		inline Gamespector(const Cookie::ECS::Coordinator& _coordinator, const Cookie::Resources::Texture* const & _nullTexture)
			: GameWindowBase	("Gamespector", true),
			  coordinator		(_coordinator),
			  nullTexture		(_nullTexture)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::GamespectorID; };
	};
}

#endif