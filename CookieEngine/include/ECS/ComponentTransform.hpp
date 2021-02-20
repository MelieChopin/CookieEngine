#ifndef __COMPONENT_TRANSFORM_HPP__
#define __COMPONENT_TRANSFORM_HPP__

#include "Mat4.hpp"

namespace Cookie
{
	namespace ECS
	{

		class Transform
		{
		public:
			Core::Math::Vec3 translation{ 0,0,0 };
			Core::Math::Vec3 rotation{ 0,0,0 };
			Core::Math::Vec3 scale{ 1,1,1 };


			Transform() {}
			Transform(const Core::Math::Vec3& _translation, const Core::Math::Vec3& _rotation, const Core::Math::Vec3& _scale) : translation{ _translation }, rotation{ _rotation }, scale{ _scale } {}
			~Transform() {}

		};

		class ComponentTransform
		{
		public:
			Transform localTRS;
			Core::Math::Mat4 globalTRS;


			ComponentTransform() { ToDefault(); }
			~ComponentTransform() {}

			void ToDefault()
			{
				localTRS.translation = {0, 0, 0};
				localTRS.rotation    = {0, 0, 0};
				localTRS.scale       = {1, 1, 1};
				globalTRS = Core::Math::Mat4::Identity();
			}
		};

	}
}

#endif