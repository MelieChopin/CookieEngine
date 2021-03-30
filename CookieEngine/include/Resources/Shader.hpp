#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <string>
#include "Core/Math/Mat4.hpp"
#include <d3dcommon.h>

class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11InputLayout;

namespace Cookie
{
	namespace Render
	{
		class Renderer;
		class RenderRemote;
	}

	namespace Resources
	{
		class Shader
		{
			protected:
				ID3D11VertexShader* VShader = nullptr;
				ID3D11PixelShader*	PShader = nullptr;
				ID3D11InputLayout*	layout	= nullptr;
				ID3D11Buffer*		CBuffer = nullptr;

			protected:
				virtual bool CompileVertex(ID3DBlob** VS) = 0;
				virtual bool CompilePixel() = 0;
				 
				virtual bool CreateLayout(ID3DBlob** VS) = 0;
				virtual bool CreateBuffer() = 0;
				 
				virtual std::string GetVertexSource() = 0;
				virtual std::string GetPixelSource() = 0;

			public:
				std::string name;

				/* CONSTRUCTORS/DESTRUCTORS */
				Shader(std::string _name) :name{ _name } {}
				virtual ~Shader() {}

				virtual void Set(const Core::Math::Mat4& projMat, const Core::Math::Mat4& viewMat) = 0;
		};
	}
}

#endif // __SHADER_HPP__
