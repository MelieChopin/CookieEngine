#ifndef __GAMEPLAY_PASS_HPP__
#define __GAMEPLAY_PASS_HPP__

#include "Render/Drawers/PlayerDrawer.hpp"

struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

namespace Cookie
{
	namespace Render
	{
		class DrawDataHandler;

		class GameplayPass
		{
			private:
				ID3D11VertexShader*			VShader{ nullptr };
				ID3D11PixelShader*			PShader{ nullptr };

				ID3D11Buffer* VCBuffer{ nullptr };
				ID3D11Buffer* PCBuffer{ nullptr };

				ID3D11RasterizerState*		rasterizerState		{ nullptr };
				ID3D11DepthStencilState*	depthStencilState	{ nullptr };
				ID3D11BlendState*			blendState			{ nullptr };
				ID3D11SamplerState*			PSampler			{ nullptr };

			public:
				PlayerDrawer				playerDrawer;

			private:

				void InitShader();
				void InitState();

			public:
				GameplayPass();
				~GameplayPass();

				void Set();
				void Draw(const DrawDataHandler& drawData);
		};
	}
}

#endif /*__GAMEPLAY_PASS_HPP__*/