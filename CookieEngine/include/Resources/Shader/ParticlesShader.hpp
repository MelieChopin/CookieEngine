#ifndef __PARTICLESSHADER_HPP__
#define __PARTICLESSHADER_HPP__

#include "Render/RendererRemote.hpp"
#include "Resources/Shader.hpp"

namespace Cookie
{
	namespace Resources
	{
		class ParticlesShader : public Shader
		{
		protected:
			ID3D11SamplerState* sampler = nullptr;

			virtual bool CompileVertex(ID3DBlob** VS)override;
			virtual bool CompilePixel()override;

			virtual bool CreateLayout(ID3DBlob** VS)override;
			virtual bool CreateBuffer()override;

			virtual std::string GetVertexSource()override;
			virtual std::string GetPixelSource()override;

			bool CreateSampler();

		public:

			/* CONSTRUCTORS/DESTRUCTORS */
			ParticlesShader(std::string _name);
			virtual ~ParticlesShader();

			virtual void Set(const Core::Math::Mat4& projMat, const Core::Math::Mat4& viewMat)override;
		};
	}
}

#endif // !__PARTICLESSHADER_HPP__
