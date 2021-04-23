#ifndef __GEOMETRY_PASS_HPP__
#define __GEOMETRY_PASS_HPP__

#include <memory>
#include "FrameBuffer.hpp"

namespace Cookie
{
	namespace Resources
	{
		class Shader;
	}

	namespace Render
	{


		class GeometryPass
		{
			private:
				std::unique_ptr<Resources::Shader> gShader{nullptr};
			public:
				FrameBuffer				posFBO;
				FrameBuffer				normalFBO;
				FrameBuffer				albedoFBO;

			public:
				GeometryPass(int width, int height);
				~GeometryPass();

				void Set(ID3D11DepthStencilView* depthStencilView);
				void Clear(const Core::Math::Vec4& clearColor);
		};
	}
}

#endif /*__GEOMETRY_PASS_HPP__*/