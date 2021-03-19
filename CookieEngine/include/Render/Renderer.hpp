#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <vector>
#include "Render/FrameBuffer.hpp"
#include "Core/Window.hpp"
#include "Render/RendererRemote.hpp"
#include "Render/RendererState.hpp"
#include "Core/Math/Mat4.hpp" 

namespace Cookie
{
	namespace ECS
	{
		class Coordinator;
	};

	namespace Resources
	{
		class ResourcesManager;
	};

	namespace Render
	{
		class Renderer
		{
			private:
				struct ID3D11Device*			device			= nullptr;
				struct IDXGISwapChain*			swapchain		= nullptr;
				struct ID3D11RenderTargetView*	backbuffer		= nullptr;
				struct ID3D11DepthStencilView*	depthBuffer		= nullptr;
				std::vector<std::unique_ptr<FrameBuffer>> frameBuffers;
				
			public:
				RendererRemote					remote;
				RendererState					state;

			private:
				bool InitDevice(Core::Window& window);
				bool CreateDrawBuffer(int width, int height);
				bool InitState(int width, int height);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Renderer(Core::Window& window);
				~Renderer();

				void AddFrameBuffer(Resources::ResourcesManager& resources);


				bool CreateBuffer(D3D11_BUFFER_DESC bufferDesc, D3D11_SUBRESOURCE_DATA data, ID3D11Buffer** buffer);
				bool CreateVertexBuffer(ID3D11VertexShader** vertexShader, ID3DBlob** VS);
				bool CreatePixelBuffer(ID3D11PixelShader** pixelShader, ID3DBlob** PS);
				
				void Draw(const Core::Math::Mat4& viewProj, ECS::Coordinator& coordinator);
				void Render();
				void Clear();
				void SetBackBuffer();
				void SetFrameBuffer(const FrameBuffer& frameBuffer);
				void ClearFrameBuffer(const FrameBuffer& frameBuffer);

				inline FrameBuffer& GetLastFrameBuffer() { return *frameBuffers[frameBuffers.size() - 1 ]; }


				void ResizeBuffer(int width, int height);

				inline ID3D11Device* GetDevice() const {return device;};

		};
	}
}

#endif /*__RENDERER_HPP__*/