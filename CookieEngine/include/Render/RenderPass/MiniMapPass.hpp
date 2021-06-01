#ifndef __MINI_MAP_PASS_HPP__
#define __MINI_MAP_PASS_HPP__

struct ID3D11InputLayout;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

#include "Drawers/MiniMapDrawer.hpp"

#define PLAYER_ARMY_COLOR 0.0f,0.0f,1.0f
#define AI1_ARMY_COLOR 1.0f,0.0f,0.0f

namespace Cookie
{
	namespace Render
	{
		class DrawDataHandler;

		class MiniMapPass
		{
		private:
			ID3D11InputLayout*	ILayout{ nullptr };
			ID3D11VertexShader* VShader{ nullptr };
			ID3D11PixelShader*	PShader{ nullptr };
			D3D11_VIEWPORT		viewport;

			
		public:
			Core::Math::Mat4 ortho;
			Core::Math::Mat4 miniMapView;

			MiniMapDrawer		miniMapDrawer;

			ID3D11Buffer*		CamCBuffer{ nullptr };
			ID3D11Buffer*		CBuffer{ nullptr };

		private:
			void InitShader();

		public:

			MiniMapPass();
			~MiniMapPass();

			void Set();
			void Draw(DrawDataHandler& drawData);
		};
	}
}

#endif /*__MINI_MAP_PASS_HPP__*/