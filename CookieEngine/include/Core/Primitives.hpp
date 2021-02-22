#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include "Resources/Mesh.hpp"

namespace Cookie
{
	namespace Core
	{
		namespace Primitives
		{
			inline std::shared_ptr<Cookie::Resources::Mesh> CreateQuad(Render::Renderer& _renderer)
			{
				std::vector<float> vertices = { -1, -1, 0, 0, 0, 0, 0, 1,
												1, -1, 0, 1, 0, 0, 0, 1,
												1, 1, 0, 1, 1, 0, 0, 1,
												-1, 1, 0, 0, 1, 0, 0, 1  };

				std::vector<unsigned int> indices = { 1, 2, 3, 3, 1, 0 };


				std::shared_ptr<Cookie::Resources::Mesh> quad = std::make_shared<Cookie::Resources::Mesh>(vertices, indices, 6, _renderer);
				quad->name = "Quad";
				return quad;
			}

			inline std::shared_ptr<Cookie::Resources::Mesh> CreateTriangle(Render::Renderer& _renderer)
			{
				std::vector<float> vertices = { -1, -1, 0, 0, 0, 0, 0, 1,
												1, -1, 0, 0, 1, 0, 0, 1,
												0, 1, 0, 0.5, 1, 0, 0, 1 };

				std::vector<unsigned int> indices = { 0, 1, 2 };

				std::shared_ptr<Cookie::Resources::Mesh> triangle = std::make_shared<Cookie::Resources::Mesh>(vertices, indices, 3, _renderer);
				triangle->name = "Triangle";
				return triangle;
			}
		}
	}
}

#endif // !__PRIMITIVES_HPP__
