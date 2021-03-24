#ifndef __COMPONENT_PHYSICS_HPP__
#define __COMPONENT_PHYSICS_HPP__

#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
    namespace ECS
    {

		class ComponentPhysics
		{
			public:
				reactphysics3d::RigidBody* physBody		= nullptr;
				std::vector<reactphysics3d::Collider*>	physCollider;

			public:
				ComponentPhysics() {}
				~ComponentPhysics() {}

			inline void AddSphereCollider(std::shared_ptr<reactphysics3d::PhysicsCommon> physCom, float radius, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::SphereShape* sphere = physCom->createSphereShape(radius);
				
				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				//physTransform.push_back(trs);

				physCollider.push_back(physBody->addCollider(sphere, trs));
			}

			inline void AddCubeCollider(std::shared_ptr<reactphysics3d::PhysicsCommon> physCom, const Core::Math::Vec3& halfExtent, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::BoxShape* cube = physCom->createBoxShape({halfExtent.x,halfExtent.y,halfExtent.z});

				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				//physTransform.push_back(trs);

				physCollider.push_back(physBody->addCollider(cube, trs));
			}

			inline void AddCapsuleCollider(std::shared_ptr<reactphysics3d::PhysicsCommon> physCom, const Core::Math::Vec2& capsuleInfo, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::CapsuleShape* capsule = physCom->createCapsuleShape(capsuleInfo.x,capsuleInfo.y);

				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				//physTransform.push_back(trs);

				physCollider.push_back(physBody->addCollider(capsule, trs));
			}

			inline void ToDefault()
			{
				physBody = nullptr;
				std::vector<reactphysics3d::Collider*>().swap(physCollider);
			}
		};

    }
}

#endif // !__COMPONENT_PHYSICS_HPP__
