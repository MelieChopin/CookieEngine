#ifndef __MINI_MAP_PASS_HPP__
#define __MINI_MAP_PASS_HPP__

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

#include <memory>
#include "Drawers/MiniMapDrawer.hpp"
#include "Drawers/MiniModelDrawer.hpp"

struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;

namespace Cookie
{
	namespace Resources
	{
		class Texture;
		class Mesh;
	}

	namespace Render
	{


		class FrameBuffer;

		class DrawDataHandler;

		class MiniMapPass
		{
		private:
			D3D11_VIEWPORT viewport;
			
		public:
			float aspectRatio = 1.0f;

			Core::Math::Mat4 ortho;
			Core::Math::Mat4 miniMapView;

			MiniMapDrawer		miniMapDrawer;
			MiniModelDrawer		miniModelDrawer;

			ID3D11DepthStencilState*	depthStencilState;
			ID3D11RasterizerState*		rasterState;

		private:
			void InitState();

		public:

			MiniMapPass();
			~MiniMapPass();

			void Draw(DrawDataHandler& drawData, const FrameBuffer& frameBuffer);
		};
	}
}

#endif /*__MINI_MAP_PASS_HPP__*/