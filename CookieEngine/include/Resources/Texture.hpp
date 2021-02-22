#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <d3d11.h>
#include <string>

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
			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Texture(Render::Renderer& renderer, const std::string& texPath);
				~Texture();


				void Set(Render::RendererRemote& remote);

		};
	}
}

#endif // !__TEXTURE_HPP__
