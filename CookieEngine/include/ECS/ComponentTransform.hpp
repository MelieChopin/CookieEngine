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

			bool trsHasChanged = {true};

			inline ComponentTransform();
			inline ComponentTransform(const Core::Math::Vec3& _translation, const Core::Math::Vec3& _rotation, const Core::Math::Vec3& _scale);
			inline ~ComponentTransform() {}

			inline void ToDefault() noexcept;
			inline void ComputeTRS();

		};

	}
}

#include "ECS/ComponentTransform.inl"

#endif