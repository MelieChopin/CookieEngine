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
			std::string			name		= "No Name";
			std::string			nameMesh	= "Null";
			std::string			nameTexture = "Null";
			std::string			nameShader	= "Null";
			std::string			nameScript	= "Null";
			std::string			filepath	= "Null";
			Core::Math::Vec3	rotation;
			Core::Math::Vec3	scale;
			int					signature	= 0;

			Prefab() {}
			Prefab(const std::string& _name) : name{ _name }  {}
		};
	}
}


#endif 