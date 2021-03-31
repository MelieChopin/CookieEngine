#ifndef __COMPONENT_TRANSFORM_HPP__
#define __COMPONENT_TRANSFORM_HPP__

#include <reactphysics3d/mathematics/Transform.h>
#include "Mat4.hpp"
#include "Quat.hpp"

namespace Cookie
{
	namespace ECS
	{

		class Transform
		{
		public:
			Core::Math::Mat4 TRS = Core::Math::Mat4::Identity();

			Core::Math::Vec3 pos{ 0,0,0 };
			Core::Math::Vec3 rot{ 0,0,0 };
			Core::Math::Vec3 scale{ 1,1,1 };


			Transform() {}
			Transform(const Core::Math::Vec3& _translation, const Core::Math::Vec3& _rotation, const Core::Math::Vec3& _scale) : pos{_translation},rot{_rotation}, scale{ _scale } {}
			
			inline void ComputeTRS() 
			{ 
				TRS = Core::Math::Mat4::TRS(pos, {Core::Math::ToRadians(rot.x),Core::Math::ToRadians(rot.y) ,Core::Math::ToRadians(rot.z) }, scale);
			}
			
			~Transform() {}

		};

		class ComponentTransform
		{
		public:
			Transform					localTRS;
			reactphysics3d::Transform	physTransform;
			reactphysics3d::Transform	oldTransform;

			Core::Math::Mat4 parentTRS;


			ComponentTransform() { ToDefault(); }
			~ComponentTransform() {}

			inline void Update(float factor)noexcept
			{
				physTransform = reactphysics3d::Transform::interpolateTransforms(oldTransform, physTransform, factor);

				reactphysics3d::Vector3		pos = physTransform.getPosition();
				reactphysics3d::Quaternion	rot = physTransform.getOrientation();

				localTRS.pos = { pos.x,pos.y,pos.z };
				localTRS.rot = Core::Math::Quat::ToEulerAngle({ rot.w,rot.x,rot.y,rot.z });

				oldTransform = physTransform;
			}

			inline void SetPhysics()noexcept
			{
				physTransform.setPosition({ localTRS.pos.x,localTRS.pos.y,localTRS.pos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(localTRS.rot);
				physTransform.setOrientation({quat.x,quat.y,quat.z,quat.w});
			}

			void ToDefault()
			{
				localTRS.pos			= {0 ,0 ,0};
				localTRS.rot			= {0 ,0 ,0};
				localTRS.scale			= {1, 1, 1};
				SetPhysics();
				oldTransform = physTransform;
				parentTRS = Core::Math::Mat4::Identity();

			}
		};

	}
}

#endif