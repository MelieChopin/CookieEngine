#ifndef __LIGHT_PASS_HPP__
#define __LIGHT_PASS_HPP__

#include "FrameBuffer.hpp"
#include "RenderPass/DirLightPass.hpp"

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
		struct	LightsArray;
		class	ShadowBuffer;

		class LightPass
		{
		private:
			DirLightPass				dirPass;

			ID3D11Buffer*				lightCBuffer		{ nullptr };
			ID3D11SamplerState*			PSampler			{ nullptr };

			ID3D11DepthStencilState*	depthStencilState	= nullptr;
			ID3D11RasterizerState*		rasterizerState		= nullptr;
			ID3D11BlendState*			blendState			= nullptr;

		public:
			FrameBuffer diffuseFBO;
			FrameBuffer specularFBO;
			

		private:
			void InitShader();
			void InitState();

		public:
			LightPass(int width, int height);
			~LightPass();

			void Set(FrameBuffer& posFBO, FrameBuffer& normalFBO, FrameBuffer& albedoFBO, const Core::Math::Vec3& camPos);
			void Draw(const LightsArray& lights, const ShadowBuffer& shadowMap);
			void Clear();
		};
	}
}

#endif /*__LIGHT_PASS_HPP__*/