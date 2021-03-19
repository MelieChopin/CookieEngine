#ifndef __COMPONENT_COLLIDER_HPP__
#define __COMPONENT_COLLIDER_HPP__

#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
    namespace ECS
    {

		class ComponentCollider
		{
			private:
			std::vector<reactphysics3d::Collider*>	physCollider;
			//std::vector<reactphysics3d::Transform>	physTransform;

			public:
				ComponentCollider() {}
				~ComponentCollider() {}

			void AddSphereCollider(std::shared_ptr<reactphysics3d::PhysicsCommon> physCom, reactphysics3d::RigidBody* body,float radius, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::SphereShape* sphere = physCom->createSphereShape(radius);
				
				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				//physTransform.push_back(trs);

				physCollider.push_back(body->addCollider(sphere, trs));
			}

			void AddCubeCollider(std::shared_ptr<reactphysics3d::PhysicsCommon> physCom, reactphysics3d::RigidBody* body, const Core::Math::Vec3& halfExtent, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::BoxShape* cube = physCom->createBoxShape({halfExtent.x,halfExtent.y,halfExtent.z});

				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				//physTransform.push_back(trs);

				physCollider.push_back(body->addCollider(cube, trs));
			}

			void AddCapsuleCollider(std::shared_ptr<reactphysics3d::PhysicsCommon> physCom, reactphysics3d::RigidBody* body, const Core::Math::Vec2& capsuleInfo, const Core::Math::Vec3& localPos, const Core::Math::Vec3& eulerAngles)
			{
				reactphysics3d::CapsuleShape* capsule = physCom->createCapsuleShape(capsuleInfo.x,capsuleInfo.y);

				reactphysics3d::Transform trs;

				trs.setPosition({ localPos.x,localPos.y,localPos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(eulerAngles);
				trs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				//physTransform.push_back(trs);

				physCollider.push_back(body->addCollider(capsule, trs));
			}

			void ToDefault()
			{
				physCollider.clear();
				//physTransform.clear();
			}
		};

    }
}

#endif // !__COMPONENT_COLLIDER_HPP__
