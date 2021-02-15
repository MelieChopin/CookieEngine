#ifndef __RENDERER_STATE_HPP__
#define __RENDERER_STATE_HPP__

#include <d3d11.h>

namespace Cookie
{
	namespace Render
	{
		class RendererState
		{
			public:
				ID3D11DepthStencilState*	depthStencilState	= nullptr;
				ID3D11RasterizerState*		rasterizerState		= nullptr;
				D3D11_VIEWPORT				viewport = {};

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				RendererState();
				~RendererState();
		};
	}
}

#endif /* __RENDERER_STATE_HPP__ */