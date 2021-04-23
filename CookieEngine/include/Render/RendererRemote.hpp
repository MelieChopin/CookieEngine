#ifndef __RENDERER_REMOTE_HPP__
#define __RENDERER_REMOTE_HPP__

#include <d3d11.h>

namespace Cookie
{
	namespace Resources
	{
		class Shader;
	}

	namespace Render
	{


		class RendererRemote
		{
			public:
				static struct	ID3D11Device*			device;
				static struct	ID3D11DeviceContext*	context;
				static class	Resources::Shader*	currentShader;


			/* CONSTRUCTORS/DESTRUCTORS */
				RendererRemote();
				~RendererRemote();
		};
	}
}

#endif /* __RENDERER_REMOTE_HPP__ */