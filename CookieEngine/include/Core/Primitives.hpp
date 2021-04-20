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
				std::vector<float> vertices = { -0.5, -0.5, 0, 0, 0, 0, 0, 1,
												 0.5, -0.5, 0, 1, 0, 0, 0, 1,
												 0.5,  0.5, 0, 1, 1, 0, 0, 1,
												-0.5,  0.5, 0, 0, 1, 0, 0, 1  };

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

			inline std::shared_ptr<Cookie::Resources::Mesh> CreateCube()
			{
				
				std::vector<float> vertices = {
				  0.5f,  0.5f,  0.5f,	0.748573, 0.750412,	 1.0f,  1.0f,  1.0f,
				  0.5f, -0.5f,  0.5f,	0.749279, 0.501284,  1.0f, -1.0f,  1.0f,
				 -0.5f, -0.5f,  0.5f,	0.999110, 0.501077, -1.0f, -1.0f,  1.0f,
				 -0.5f,  0.5f,  0.5f,	0.999455, 0.750380, -1.0f,  1.0f,  1.0f,
				 -0.5f, -0.5f, -0.5f,	0.250471, 0.500702, -1.0f, -1.0f, -1.0f,
				 -0.5f,  0.5f, -0.5f,	0.249682, 0.749677, -1.0f,  1.0f, -1.0f,
				 -0.5f,  0.5f,  0.5f,	0.001085, 0.750380, -1.0f,  1.0f,  1.0f,
				 -0.5f, -0.5f,  0.5f,	0.001517, 0.499994, -1.0f, -1.0f,  1.0f,
				  0.5f, -0.5f, -0.5f,	0.499422, 0.500239,  1.0f, -1.0f, -1.0f,
				  0.5f,  0.5f, -0.5f,	0.500149, 0.750166,  1.0f,  1.0f, -1.0f,
				 -0.5f, -0.5f, -0.5f,	0.250471, 0.500702, -1.0f, -1.0f, -1.0f,
				 -0.5f,  0.5f, -0.5f,	0.249682, 0.749677, -1.0f,  1.0f, -1.0f,
				  0.5f, -0.5f,  0.5f,	0.749279, 0.501284,  1.0f, -1.0f,  1.0f,
				  0.5f,  0.5f,  0.5f,	0.748573, 0.750412,  1.0f,  1.0f,  1.0f,
				  0.5f, -0.5f, -0.5f,	0.499422, 0.500239,  1.0f, -1.0f, -1.0f,
				  0.5f,  0.5f, -0.5f,	0.500149, 0.750166,  1.0f,  1.0f, -1.0f,
				  0.5f,  0.5f,  0.5f,	0.748573, 0.750412,  1.0f,  1.0f,  1.0f,
				 -0.5f,  0.5f,  0.5f,	0.748355, 0.998230, -1.0f,  1.0f,  1.0f,
				  0.5f,  0.5f, -0.5f,	0.500149, 0.750166,  1.0f,  1.0f, -1.0f,
				 -0.5f,  0.5f, -0.5f,	0.500193, 0.998728, -1.0f,  1.0f, -1.0f,
				  0.5f, -0.5f,  0.5f,	0.749279, 0.501284,  1.0f, -1.0f,  1.0f,
				  0.5f, -0.5f, -0.5f,	0.499422, 0.500239,  1.0f, -1.0f, -1.0f,
				 -0.5f, -0.5f, -0.5f,	0.498993, 0.250415, -1.0f, -1.0f, -1.0f,
				 -0.5f, -0.5f,  0.5f,	0.748953, 0.250920, -1.0f, -1.0f,  1.0f
				 };

				std::vector<unsigned int> indices = { 
				2 , 1 , 0 ,
				3 , 2 , 0 ,
				6 , 5 , 4 ,
				7 , 6 , 4 ,
				10 , 9 , 8 ,
				10 , 11 , 9 ,
				14 , 13 , 12 ,
				14 , 15 , 13 ,
				18 , 17 , 16 ,
				18 , 19 , 17 ,
				22 , 21 , 20 ,
				23 , 22 , 20};

				std::shared_ptr<Cookie::Resources::Mesh> cube = std::make_shared<Cookie::Resources::Mesh>("Cube", vertices, indices, 36);
				return cube;
			}
		}
	}
}

#endif // !__PRIMITIVES_HPP__
