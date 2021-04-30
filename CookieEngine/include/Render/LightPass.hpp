#ifndef __LIGHT_PASS_HPP__
#define __LIGHT_PASS_HPP__

#include "Light.hpp"
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
			ID3D11VertexShader* dirVShader	{ nullptr };
			ID3D11PixelShader*	dirPShader	{ nullptr };
			ID3D11Buffer*		dirCBuffer	{ nullptr };

			ID3D11Buffer*		lightCBuffer{ nullptr };

			ID3D11SamplerState* PSampler	{ nullptr };

			ID3D11DepthStencilState*	depthStencilState	= nullptr;
			ID3D11RasterizerState*		rasterizerState		= nullptr;
			ID3D11BlendState*			blendState			= nullptr;
		public:
			LightsArray lights;

		private:
			void InitShader();
			void InitState();

		public:
			LightPass();
			~LightPass();

			void Set(FrameBuffer& posFBO, FrameBuffer& normalFBO, FrameBuffer& albedoFBO, const Core::Math::Vec3& camPos);
			void Draw(FrameBuffer& fbo);
		};
	}
}

#endif /*__LIGHT_PASS_HPP__*/