#ifndef __COMPONENT_TRANSFORM_HPP__
#define __COMPONENT_TRANSFORM_HPP__

#include "Mat4.hpp"

namespace Cookie
{
	namespace ECS
	{
		class ComponentTransform
		{
		public:
			Core::Math::Mat4 TRS = Core::Math::Mat4::Identity();

			Core::Math::Vec3 pos{ 0,0,0 };
			Core::Math::Vec3 rot{ 0,0,0 };
			Core::Math::Vec3 scale{ 1,1,1 };

			inline void ComputeTRS()
			{
				TRS = Core::Math::Mat4::TRS(pos, { Core::Math::ToRadians(rot.x),Core::Math::ToRadians(rot.y) ,Core::Math::ToRadians(rot.z) }, scale);
			}

			inline ComponentTransform() { ToDefault(); }
			inline ComponentTransform(const Core::Math::Vec3& _translation, const Core::Math::Vec3& _rotation, const Core::Math::Vec3& _scale) : pos{ _translation }, rot{ _rotation }, scale{ _scale }{}
			inline ~ComponentTransform() {}

			inline void ToDefault()noexcept
			{
				pos			= {0 ,0 ,0};
				rot			= {0 ,0 ,0};
				scale		= {1, 1, 1};

				TRS = Core::Math::Mat4::Identity();
			}
		};

	}
}

#endif