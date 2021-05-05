#ifndef __SHADOW_BUFFER_HPP__
#define __SHADOW_BUFFER_HPP__

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;

namespace Cookie
{
	namespace Render
	{
		class ShadowBuffer
		{
		public:
			ID3D11Texture2D*			texBuffer{ nullptr };
			ID3D11ShaderResourceView*	shaderResource{ nullptr };
			ID3D11DepthStencilView*		depthStencilView{ nullptr };

		private:
			bool CreateTexture(int width, int height);
			bool CreateShaderResource();
			bool CreateDepthStencilView();

		public:
			/* CONSTRUCTORS/DESTRUCTORS */
			ShadowBuffer(int width, int height);
			~ShadowBuffer();

			void Resize(int width, int height);
		};
	}
}

#endif // !__SHADOW_BUFFER_HPP__
