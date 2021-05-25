#ifndef __GAMEPLAY_PASS_HPP__
#define __GAMEPLAY_PASS_HPP__


struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11SamplerState;

namespace Cookie
{
	namespace Render
	{
		struct DirLight;
		class ShadowBuffer;

		class GameplayPass
		{
		private:
			ID3D11VertexShader* VShader{ nullptr };
			ID3D11PixelShader* PShader{ nullptr };

			ID3D11Buffer* CBuffer{ nullptr };
			ID3D11SamplerState* CSampler{ nullptr };

		private:
			void InitShader();

		public:
			GameplayPass();
			~GameplayPass();

			void Set(const DirLight& dirLight, const ShadowBuffer& shadowMap, ID3D11Buffer** lightCBuffer);
		};
	}
}

#endif /*__GAMEPLAY_PASS_HPP__*/