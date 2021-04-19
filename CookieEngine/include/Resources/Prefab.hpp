#ifndef __PREFAB_HPP__
#define __PREFAB_HPP__

#include <string>

namespace reactphysics3d
{
	class RigidBody;
	class Collider;
}

namespace Cookie
{
	namespace Core
	{
		namespace Math
		{
			union Vec3;
		}
	}

	namespace Resources
	{
		class Prefab
		{
		public:
			std::string									name		= "No Name";
			std::string									nameMesh	= "Null";
			std::string									nameTexture = "Null";
			std::string									nameShader	= "Null";
			std::string									nameScript	= "Null";
			std::string									filepath	= "Null";
			Core::Math::Vec3							rotation;
			Core::Math::Vec3							scale;
			::reactphysics3d::RigidBody*				rigidBody = nullptr;
			std::vector<::reactphysics3d::Collider*>	colliders;
			int											signature = 0;

			Prefab() {}
			Prefab(const std::string& _name) : name{ _name }  {}
		};
	}
}


#endif 