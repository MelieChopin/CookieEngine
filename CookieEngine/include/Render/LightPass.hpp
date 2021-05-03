#ifndef __LIGHT_PASS_HPP__
#define __LIGHT_PASS_HPP__

#include "Light.hpp"
#include "FrameBuffer.hpp"
#include "DirLightPass.hpp"

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
			DirLightPass				dirLight;

			ID3D11Buffer*				lightCBuffer		{ nullptr };
			ID3D11SamplerState*			PSampler			{ nullptr };

			ID3D11DepthStencilState*	depthStencilState	= nullptr;
			ID3D11RasterizerState*		rasterizerState		= nullptr;
			ID3D11BlendState*			blendState			= nullptr;

		public:
			FrameBuffer diffuseFBO;
			FrameBuffer specularFBO;
			LightsArray lights;

		private:
			void InitShader();
			void InitState();

		public:
			LightPass(int width, int height);
			~LightPass();

			void Set(FrameBuffer& posFBO, FrameBuffer& normalFBO, const Core::Math::Vec3& camPos);
			void Draw();
		};
	}
}

#endif /*__LIGHT_PASS_HPP__*/