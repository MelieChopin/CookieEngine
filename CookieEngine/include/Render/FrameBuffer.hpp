#ifndef __FRAME_BUFFER_HPP__
#define __FRAME_BUFFER_HPP__

#include <d3d11.h>
#include <memory>

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
		class Shader;
		class Mesh;
	}

	namespace Render
	{
		class Renderer;
		struct RendererRemote;

		class FrameBuffer
		{
			public:
				ID3D11Texture2D*			texBuffer			{ nullptr };
				ID3D11ShaderResourceView*	shaderResource		{ nullptr };
				ID3D11RenderTargetView*		renderTargetView	{ nullptr };
				DXGI_FORMAT					format				{ DXGI_FORMAT_UNKNOWN };


			private:
				bool CreateTexture(int width, int height);
				bool CreateShaderResource();
				bool CreateRenderTargetView();

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				FrameBuffer(int width, int height, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_UNORM);
				~FrameBuffer();

				void Resize(int width, int height);
		};
	}
}

#endif // !__FRAME_BUFFER_HPP__
