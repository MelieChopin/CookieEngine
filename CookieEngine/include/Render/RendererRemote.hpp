#ifndef __RENDERER_REMOTE_HPP__
#define __RENDERER_REMOTE_HPP__

namespace Cookie
{
	namespace Render
	{
		class RendererRemote
		{
			public:
				static struct ID3D11Device* device;
				static struct ID3D11DeviceContext* context;


			/* CONSTRUCTORS/DESTRUCTORS */
				RendererRemote();
				~RendererRemote();
		};
	}
}

#endif /* __RENDERER_REMOTE_HPP__ */