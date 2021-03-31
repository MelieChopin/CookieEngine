#ifndef __COMPONENT_EDITOR_HPP__
#define __COMPONENT_EDITOR_HPP__

#include "Physics/PhysicsHandle.hpp"
#include "ECS/ComponentTransform.hpp"
#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
	namespace ECS
	{
		#define EDIT_OFFSET 0.0001f 

		class ComponentEditor
		{
			public:
			ComponentEditor(){}
			~ComponentEditor(){}
			

			const ECS::Transform*	editTrs { nullptr };
			rp3d::CollisionBody*	body	{ nullptr };
			rp3d::Collider*			collider{ nullptr };
			Core::Math::Vec3		AABB	{ 0.0f,0.0f,0.0f };

			inline void InitComponent(const ECS::Transform& trs)
			{
				editTrs = &trs;
				reactphysics3d::Transform newTrs;
				newTrs.setPosition({ trs.pos.x,trs.pos.y,trs.pos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(trs.rot);
				newTrs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				body = Physics::PhysicsHandle::editWorld->createCollisionBody(newTrs);
			}

			inline void Update()
			{
				reactphysics3d::Transform newTrs;
				newTrs.setPosition({ editTrs->pos.x,editTrs->pos.y,editTrs->pos.z });
				Core::Math::Quat quat = Core::Math::Quat::ToQuat(editTrs->rot);
				newTrs.setOrientation({ quat.x,quat.y,quat.z,quat.w });

				body->setTransform(newTrs);
			}

			inline void MakeCollider()
			{
				if (collider)
					body->removeCollider(collider);

				rp3d::BoxShape* box = Physics::PhysicsHandle::physCom->createBoxShape({std::abs(AABB.x * editTrs->scale.x) + EDIT_OFFSET,std::abs(AABB.y * editTrs->scale.y) + EDIT_OFFSET ,std::abs(AABB.z * editTrs->scale.z) + EDIT_OFFSET });
				collider			= body->addCollider(box, rp3d::Transform::identity());
			}
		};
	}
}

#endif // !__COMPONENT_EDITOR_HPP__
