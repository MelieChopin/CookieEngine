#ifndef __COMPONENT_PHYSICS_HPP__
#define __COMPONENT_PHYSICS_HPP__

#include <reactphysics3d/reactphysics3d.h>
#include "Physics/PhysicsHandle.hpp"

namespace Cookie
{
    namespace ECS
    {

		class ComponentPhysics
		{
			public:
				reactphysics3d::RigidBody* physBody		= nullptr;
				std::vector<reactphysics3d::Collider*>	physColliders;

			public:
				ComponentPhysics() {}
				~ComponentPhysics() {}

			inline void AddSphereCollider(float radius, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::SphereShape* sphere = Physics::PhysicsHandle().physCom->createSphereShape(radius);
				
				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				physColliders.push_back(physBody->addCollider(sphere, trs));
			}

			inline void AddCubeCollider(const Core::Math::Vec3& halfExtent, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::BoxShape* cube = Physics::PhysicsHandle().physCom->createBoxShape({halfExtent.x,halfExtent.y,halfExtent.z});

				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				physColliders.push_back(physBody->addCollider(cube, trs));
			}

			inline void AddCapsuleCollider(const Core::Math::Vec2& capsuleInfo, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::CapsuleShape* capsule = Physics::PhysicsHandle().physCom->createCapsuleShape(capsuleInfo.x,capsuleInfo.y);

				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				physColliders.push_back(physBody->addCollider(capsule, trs));
			}

			inline void ToDefault()
			{
				physBody = nullptr;
				std::vector<reactphysics3d::Collider*>().swap(physColliders);
			}
		};

    }
}

#endif // !__COMPONENT_PHYSICS_HPP__
