#ifndef __RENDERER_REMOTE_HPP__
#define __RENDERER_REMOTE_HPP__

namespace Cookie
{
	namespace Render
	{
		class RendererRemote
		{
			public:
				class ID3D11DeviceContext* context = nullptr;


			/* CONSTRUCTORS/DESTRUCTORS */
				RendererRemote();
				~RendererRemote();
		};
	}
}

#endif /* __RENDERER_REMOTE_HPP__ */