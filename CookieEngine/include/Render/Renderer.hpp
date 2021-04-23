#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include "Core/Window.hpp"
#include "Render/RendererRemote.hpp"
#include "Render/RendererState.hpp"

#include <vector>

namespace Cookie
{
	class Game;

	namespace Resources
	{
		class ResourcesManager;
	};

	namespace Render
	{
		class Camera;
		class FrameBuffer;

		class Renderer
		{
			private:
				
				struct IDXGISwapChain*			swapchain		= nullptr;
				struct ID3D11RenderTargetView*	backbuffer		= nullptr;
				struct ID3D11DepthStencilView*	depthBuffer		= nullptr;
				std::vector<std::unique_ptr<FrameBuffer>> frameBuffers;
				
			public:
				Core::Window					window;
				RendererRemote					remote;
				RendererState					state;

			private:
				bool InitDevice(Core::Window& window);
				bool CreateDrawBuffer(int width, int height);
				bool InitState(int width, int height);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Renderer();
				~Renderer();

				void AddFrameBuffer(Resources::ResourcesManager& resources);


				bool CreateBuffer(D3D11_BUFFER_DESC bufferDesc, D3D11_SUBRESOURCE_DATA data, ID3D11Buffer** buffer);
				
				void Draw(const Camera* cam, Game& game);
				void Render();
				void Clear();
				void SetBackBuffer();
				void SetFrameBuffer(const FrameBuffer& frameBuffer);
				void ClearFrameBuffer(const FrameBuffer& frameBuffer);

				inline FrameBuffer& GetLastFrameBuffer() { return *frameBuffers[frameBuffers.size() - 1 ]; }

				void ResizeBuffer(int width, int height);
		};
	}
}

#endif /*__RENDERER_HPP__*/