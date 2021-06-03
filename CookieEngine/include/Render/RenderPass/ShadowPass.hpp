#ifndef __SHADOW_PASS_HPP__
#define __SHADOW_PASS_HPP__

struct ID3D11VertexShader;
struct ID3D11Buffer;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct D3D11_VIEWPORT;

#include <vector>
#include <ShadowBuffer.hpp>

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

	namespace Resources
	{
		class Map;
	}

	namespace Render
	{
		struct LightsArray;
		class DrawDataHandler;
		class Camera;

		class ShadowPass
		{

		private:
			ID3D11VertexShader*			VShader{ nullptr };
			ID3D11Buffer*				CBuffer{ nullptr };

			ID3D11DepthStencilState*	depthStencilState	= nullptr;
			ID3D11RasterizerState*		rasterizerState		= nullptr;
			ID3D11BlendState*			blendState			= nullptr;
			D3D11_VIEWPORT				shadowViewport;
			Core::Math::Mat4			proj;
			
		public:
			ShadowBuffer				shadowMap;

		private:
			void InitShader();
			void InitState();

		public:
			ShadowPass();
			~ShadowPass();

			void Set();
			void Draw(DrawDataHandler& drawData);
			void Clear();
		};
	}
}

#endif /*__SHADOW_PASS_HPP__*/