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
		class RendererRemote;

		class FrameBuffer
		{
			private:
				std::shared_ptr<Cookie::Resources::Mesh>	quad				{ nullptr };
				std::shared_ptr<Cookie::Resources::Shader>	shader				{ nullptr };
				ID3D11Texture2D*							texBuffer			{ nullptr };
				ID3D11ShaderResourceView*					shaderResource		{ nullptr };
				ID3D11RenderTargetView*						renderTargetView	{ nullptr };


			private:
				bool CreateTexture(int width, int height);
				bool CreateShaderResource();
				bool CreateRenderTargetView();

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				FrameBuffer(Resources::ResourcesManager& _resources, int width, int height);
				~FrameBuffer();

				void Resize(int width, int height);
				void Draw();

				inline ID3D11Texture2D*const*			GetTexture()const			{ return &texBuffer; }
				inline ID3D11ShaderResourceView*const*	GetShaderResource()const	{ return &shaderResource; }
				inline ID3D11RenderTargetView*const*	GetRenderTarget()const		{ return &renderTargetView; }


		};
	}
}

#endif // !__FRAME_BUFFER_HPP__
