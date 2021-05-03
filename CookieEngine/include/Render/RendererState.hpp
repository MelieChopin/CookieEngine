#ifndef __RENDERER_STATE_HPP__
#define __RENDERER_STATE_HPP__

#include <d3d11.h>
#include "Core/Math/Vec4.hpp"

namespace Cookie
{
	namespace Render
	{
		struct RendererState
		{
				ID3D11DepthStencilState*	depthStencilState	= nullptr;
				ID3D11RasterizerState*		rasterizerState		= nullptr;
				D3D11_VIEWPORT				viewport = {};
				Core::Math::Vec4			clearColor = { 0.0f,0.0f,0.0f,0.0f };

				RendererState()
				{

				};

				RendererState(const RendererState& _state):
					depthStencilState {_state.depthStencilState},
					rasterizerState {_state.rasterizerState},
					viewport {_state.viewport},
					clearColor {_state.clearColor}
				{

				};

				~RendererState()
				{

				};
		};
	}
}

#endif /* __RENDERER_STATE_HPP__ */