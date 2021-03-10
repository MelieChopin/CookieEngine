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
				bool CreateTextureFromColor(Render::Renderer& renderer,const Core::Math::Vec4& color);
				bool CreateShaderResource(Render::Renderer& renderer);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Texture(Render::Renderer& renderer, const std::string& texPath);
				Texture(Render::Renderer& renderer, const std::string& texName, const Core::Math::Vec4& color);
				~Texture();


				void Set(Render::RendererRemote& remote);

				inline ID3D11ShaderResourceView* GetResourceView() const { return shaderResourceView; };
		};
	}
}

#endif // !__TEXTURE_HPP__
