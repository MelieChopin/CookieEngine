#ifndef __MINIMAP_W_HPP__
#define __MINIMAP_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class Map; class Texture; class ResourcesManager; }
namespace Cookie::Render	{ class FrameBuffer; class Camera; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int MinimapID = 1 << 2; }

	class Minimap final : public GameWindowBase
	{
		const Cookie::Resources::Texture* const minimapBg;

		const Cookie::Render::FrameBuffer& minimapBuffer;

		Cookie::Render::Camera* const camera;
		const Cookie::Resources::Map&	map;

	public:
		Minimap(Cookie::Resources::ResourcesManager& _resources, const Cookie::Render::FrameBuffer& _minimapBuffer, Cookie::Render::Camera* const _camera, const Cookie::Resources::Map& _map);

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::MinimapID; };
	};
}

#endif // !__MINIMAP_W_HPP__
