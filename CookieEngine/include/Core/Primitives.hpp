#ifndef __PRIMITIVES_HPP__
#define __PRIMITIVES_HPP__

#include "Resources/Mesh.hpp"

namespace Cookie
{
	namespace Core
	{
		namespace Primitives
		{
			inline std::shared_ptr<Cookie::Resources::Mesh> CreateQuad()
			{
				std::vector<float> vertices = { -1, -1, 0, 0, 0, 0, 0, 1,
												 1, -1, 0, 1, 0, 0, 0, 1,
												 1,  1, 0, 1, 1, 0, 0, 1,
												-1,  1, 0, 0, 1, 0, 0, 1  };

				std::vector<unsigned int> indices = {  1, 0, 3, 2, 1, 3 };


				std::shared_ptr<Cookie::Resources::Mesh> quad = std::make_shared<Cookie::Resources::Mesh>("Quad", vertices, indices, 6);
				return quad;
			}

			inline std::shared_ptr<Cookie::Resources::Mesh> CreateTriangle()
			{
				std::vector<float> vertices = { -1, -1, 0, 0,   0, 0, 0, 1,
												 1, -1, 0, 0,   1, 0, 0, 1,
												 0,  1, 0, 0.5, 1, 0, 0, 1 };

				std::vector<unsigned int> indices = { 0, 1, 2 };

				std::shared_ptr<Cookie::Resources::Mesh> triangle = std::make_shared<Cookie::Resources::Mesh>("Triangle",vertices, indices, 3);
				return triangle;
			}
		}
	}
}

#endif // !__PRIMITIVES_HPP__
