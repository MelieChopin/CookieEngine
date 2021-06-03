#ifndef __COMPOSE_PASS_HPP__
#define __COMPOSE_PASS_HPP__

namespace Cookie
{
	namespace Render
	{
		class FrameBuffer;

		class ComposePass
		{
			private:
				ID3D11VertexShader*			VShader				{ nullptr };
				ID3D11PixelShader*			PShader				{ nullptr };
				ID3D11SamplerState*			PSampler			{ nullptr };


				ID3D11DepthStencilState*	depthStencilState	= nullptr;
				ID3D11RasterizerState*		rasterizerState		= nullptr;
				ID3D11BlendState*			blendState			= nullptr;
			private:
				void InitShader();
				void InitState();

			public:
				ComposePass();
				~ComposePass();

				void Set(FrameBuffer& diffuse, FrameBuffer& specular, FrameBuffer& albedo);
				void Draw();
		};
	}
}

#endif // !__COMPOSE_PASS_HPP__
