#ifndef __MINIMAP_W_HPP__
#define __MINIMAP_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Render	{ class FrameBuffer; class Camera; }
namespace Cookie::Resources { class Map; class ResourcesManager; }


namespace Cookie::UIwidget
{
	namespace GameWidgetID
	{ constexpr int MinimapID = 1 << 2; }

	class Minimap final : public GameWindowBase
	{
		const Cookie::Render::FrameBuffer& minimapBuffer;

		Cookie::Render::Camera* const camera;
		const Cookie::Resources::Map&	map;

		Cookie::Resources::ResourcesManager& resources;

	public:
		inline Minimap(const Cookie::Render::FrameBuffer& _minimapBuffer, Cookie::Render::Camera* const _camera, const Cookie::Resources::Map& _map, Cookie::Resources::ResourcesManager& _resources)
			: GameWindowBase("Minimap", true),
			  minimapBuffer	(_minimapBuffer),
			  camera		(_camera),
			  map			(_map),
			  resources		(_resources)
		{}

		void WindowDisplay() override;

		inline int GetID() override { return GameWidgetID::MinimapID; };
	};
}

#endif // !__MINIMAP_W_HPP__
