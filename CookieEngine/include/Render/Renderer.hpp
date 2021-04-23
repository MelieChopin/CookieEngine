#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__


#include "Core/Window.hpp"
#include "Resources/Shader/FBODrawShader.hpp"
#include "Render/RendererRemote.hpp"
#include "Render/RendererState.hpp"
#include "Render/GeometryPass.hpp"

#include "Light.hpp"

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


		class Renderer
		{
			private:
				struct IDXGISwapChain*				swapchain		= nullptr;
				struct ID3D11RenderTargetView*		backbuffer		= nullptr;
				struct ID3D11DepthStencilView*		depthBuffer		= nullptr;
				
					
			public:
				Core::Window	window;
				RendererRemote	remote;
				RendererState	state;
				GeometryPass	gPass;
				LightsArray		lights;

			private:
				Resources::FBODrawShader			fboDrawer{ "FBODrawer_Shader" };

			private:
				RendererRemote InitDevice(Core::Window& window);
				bool CreateDrawBuffer(int width, int height);
				RendererState InitState(int width, int height);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Renderer();
				~Renderer();
				
				void Draw(const Camera* cam, Game& game, FrameBuffer& fbo);
				void Render();
				void Clear();
				void SetBackBuffer();
				void DrawFrameBuffer(FrameBuffer& fbo);
				void ClearFrameBuffer(FrameBuffer& fbo);

				void ResizeBuffer(int width, int height);
		};
	}
}

#endif /*__RENDERER_HPP__*/