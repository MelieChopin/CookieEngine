#ifndef __PHYSICS_SHADER_HPP__
#define __PHYSICS_SHADER_HPP__

#include "Render/RendererRemote.hpp"
#include "Resources/Shader.hpp"

namespace Cookie
{
	namespace Resources
	{
		class PhysicsShader : public Shader
		{
			protected:


				virtual bool CompileVertex(ID3DBlob** VS)override;
				virtual bool CompilePixel()override;

				virtual bool CreateLayout(ID3DBlob** VS)override;
				virtual bool CreateBuffer()override;

				virtual std::string GetVertexSource()override;
				virtual std::string GetPixelSource()override;

			public:

				/* CONSTRUCTORS/DESTRUCTORS */
				PhysicsShader(std::string _name);
				virtual ~PhysicsShader();

				virtual void Set(const Core::Math::Mat4& projMat, const Core::Math::Mat4& viewMat)override;
		};
	}
}


#endif // !__PHYSICS_SHADER_HPP__