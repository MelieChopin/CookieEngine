#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "Core/Window.hpp"
#include "Render/RendererRemote.hpp"
#include "Render/RendererState.hpp"

namespace Cookie
{
	namespace Render
	{
		class Renderer
		{
		private:
			class ID3D11Device*				device		= nullptr;
			class IDXGISwapChain*			swapchain	= nullptr;
			class ID3D11RenderTargetView*	backbuffer	= nullptr;
			class ID3D11DepthStencilView*	depthBuffer = nullptr;
			
		public:
			RendererRemote					remote;
			RendererState					state;

		private:
			bool InitDevice(Core::Window& window);
			bool CreateDrawBuffer();
			bool InitState();

		public:
			/* CONSTRUCTORS/DESTRUCTORS */
			Renderer(Core::Window& window);
			~Renderer();


			bool CreateBuffer(D3D11_BUFFER_DESC bufferDesc, D3D11_SUBRESOURCE_DATA data, ID3D11Buffer** buffer);
			bool CreateVertexBuffer(ID3D11VertexShader** vertexShader, ID3DBlob** VS);
			bool CreatePixelBuffer(ID3D11PixelShader** pixelShader, ID3DBlob** PS);
			
			void Render();
			void Clear();


			inline ID3D11Device* GetDevice() const {return device;};

		};
	}
}

#endif /*__RENDERER_HPP__*/