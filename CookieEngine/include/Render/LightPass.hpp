#ifndef __LIGHT_PASS_HPP__
#define __LIGHT_PASS_HPP__

#include "FrameBuffer.hpp"

namespace Cookie
{
	namespace Core::Math
	{
		union Vec4;
		union Mat4;
	}

	namespace ECS
	{
		class Coordinator;
	}

	namespace Render
	{
		class LightPass
		{
		private:
			ID3D11InputLayout*	ILayout{ nullptr };

			ID3D11VertexShader* dirVShader{ nullptr };
			ID3D11PixelShader*	dirPShader{ nullptr };

			ID3D11SamplerState* PSampler{ nullptr };

			ID3D11DepthStencilState*	depthStencilState	= nullptr;
			ID3D11RasterizerState*		rasterizerState		= nullptr;

			ID3D11Buffer*				CBuffer{ nullptr };

		private:
			void InitShader();
			void InitState();

		public:
			LightPass(int width, int height);
			~LightPass();

			void Set(ID3D11DepthStencilView* depthStencilView);
			void Draw(const Core::Math::Mat4& viewProj, const ECS::Coordinator& coordinator);
			void Clear(const Core::Math::Vec4& clearColor);
		};
	}
}

#endif /*__LIGHT_PASS_HPP__*/