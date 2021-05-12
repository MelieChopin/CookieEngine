#ifndef __DIR_LIGHT_PASS_HPP__
#define __DIR_LIGHT_PASS_HPP__


struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11SamplerState;

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
		struct DirLight;

		class DirLightPass
		{
		private:
			ID3D11VertexShader*	VShader{ nullptr };
			ID3D11PixelShader*	PShader{ nullptr };

			ID3D11Buffer*		CBuffer{ nullptr };
			ID3D11SamplerState*	CSampler{nullptr};

		private:
			void InitShader();

		public:
			DirLightPass();
			~DirLightPass();

			void Set(ID3D11Buffer** lightCBuffer)const;
			void Write(const DirLight& dirLight);
		};
	}
}

#endif /*__DIR_LIGHT_PASS_HPP__*/