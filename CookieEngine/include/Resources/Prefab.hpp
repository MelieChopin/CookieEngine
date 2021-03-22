#ifndef __PREFAB_HPP__
#define __PREFAB_HPP__

#include <string>
#include "../Core/Math/Vec3.hpp"

namespace Cookie
{
	namespace Resources
	{
		class Prefab
		{
		public:
			std::string			name = "No Name";

			std::string			nameMesh = "No Mesh";
			std::string			nameTexture = "No Testure";
			std::string			nameShader = "No Shader";
			Core::Math::Vec3	rotation;
			Core::Math::Vec3	scale;
			std::string			nameScript = "No Script";

			Prefab() {}
			Prefab(const std::string& _name) : name{ _name }  {}
		};
	}
}


#endif 