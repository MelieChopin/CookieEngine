#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include <d3dcommon.h>

class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11InputLayout;

namespace Cookie
{
	namespace Render
	{
		class Renderer;
	}

	namespace Resources
	{
		class Shader
		{
			private:
				ID3D11VertexShader* VShader = nullptr;
				ID3D11PixelShader*	PShader = nullptr;
				ID3D11InputLayout*	layout	= nullptr;

			private:
				bool CompileDefaultVertex(Render::Renderer& _renderer,ID3DBlob** VS);
				bool CompileDefaultPixel(Render::Renderer& _renderer);

				bool CreateDefaultLayout(Render::Renderer& _renderer,ID3DBlob** VS);

				std::string GetDefaultVertexSource();
				std::string GetDefaultPixelSource();

				bool CompileVertex(Render::Renderer& _renderer, ID3DBlob** VS, std::string VShaderPath);
				bool CompilePixel(Render::Renderer& _renderer, std::string PShaderPath);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Shader(Render::Renderer& _renderer);
				Shader(Render::Renderer& _renderer, std::string VShaderPath, std::string PShaderPath);
				~Shader();
		};
	}
}

#endif // __SHADER_HPP__
