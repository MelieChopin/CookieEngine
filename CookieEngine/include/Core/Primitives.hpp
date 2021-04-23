#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include "Resources/Mesh.hpp"

namespace Cookie
{
	namespace Core
	{
		namespace Primitives
		{
			struct DebugVertex
			{
				Math::Vec3	vertex = {0.0f,0.0f,0.0f};
				uint32_t			color = 0;

				DebugVertex(const Math::Vec3& _vertex, uint32_t _color) :vertex{ _vertex }, color{_color} {}
			};

			inline std::shared_ptr<Resources::Mesh> CreateQuad();

			inline std::vector<DebugVertex> CreateLine(const Math::Vec3& start, const Math::Vec3& end, uint32_t color1, uint32_t color2);

			inline std::shared_ptr<Resources::Mesh> CreateTriangle();

			inline std::shared_ptr<Resources::Mesh> CreateCube();
		}
	}
}


#include "Primitives.inl"

#endif // !__PRIMITIVES_HPP__
