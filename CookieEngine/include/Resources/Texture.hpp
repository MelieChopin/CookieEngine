#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <d3d11.h>
#include <string>

#include "Vec4.hpp"


namespace Cookie
{
	namespace Render
	{
		class Renderer;
		class RendererRemote;
	}

	namespace Resources
	{
		class Texture
		{
			private:
				ID3D11Resource*				texture				= nullptr;
				ID3D11ShaderResourceView*	shaderResourceView	= nullptr;

			public:
				std::string name;

			private:
				bool CreateTextureFromColor(const Core::Math::Vec4& color);
				bool CreateShaderResource();

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Texture(const std::string& texPath);
				Texture(const std::string& texName, const Core::Math::Vec4& color);
				~Texture();


				void Set();

				inline ID3D11ShaderResourceView* GetResourceView() const { return shaderResourceView; };
		};
	}
}

#endif // !__TEXTURE_HPP__
