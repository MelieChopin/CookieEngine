#ifndef __LIGHT_PASS_HPP__
#define __LIGHT_PASS_HPP__

#include "FrameBuffer.hpp"
#include "RenderPass/DirLightPass.hpp"
#include "RenderPass/PointLightPass.hpp"

namespace Cookie
{
	namespace Render
	{
		struct	LightsArray;
		class	ShadowBuffer;
		class	DrawDataHandler;

		class LightPass
		{
		private:
			DirLightPass				dirPass;
			PointLightPass				pointPass;

			ID3D11Buffer*				lightCBuffer		{ nullptr };
			ID3D11SamplerState*			PSampler			{ nullptr };

			ID3D11DepthStencilState*	depthStencilState	= nullptr;

			ID3D11RasterizerState*		rasterizerState		= nullptr;
			ID3D11BlendState*			blendState			= nullptr;

			ID3D11RasterizerState*		volumeRasterState	= nullptr;

		public:
			FrameBuffer diffuseFBO;
			FrameBuffer specularFBO;
			

		private:
			void InitShader();
			void InitState();

		public:
			LightPass(int width, int height);
			~LightPass();

			void Set(FrameBuffer& posFBO, FrameBuffer& normalFBO, FrameBuffer& albedoFBO);
			void Draw(const LightsArray& lights, const ShadowBuffer& shadowMap, const DrawDataHandler& drawData);
			void Clear();
		};
	}
}

#endif /*__LIGHT_PASS_HPP__*/