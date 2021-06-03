#ifndef __PHYSICS_HANDLE_HPP__
#define __PHYSICS_HANDLE_HPP__

#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
	namespace Physics
	{
		class PhysicsHandle
		{
			public:
				inline static std::unique_ptr<rp3d::PhysicsCommon>			physCom{nullptr};
				inline static rp3d::PhysicsWorld*							physSim{nullptr};
				inline static rp3d::PhysicsWorld*							editWorld{nullptr};

				static void Init()
				{
					physCom = std::make_unique<rp3d::PhysicsCommon>();
					physSim = PhysicsHandle::physCom->createPhysicsWorld();
				}

				static void Terminate()
				{
					int max = physSim->getNbRigidBodies();
					for (int i = 0; i < max; i++)
					{
						physSim->destroyRigidBody(physSim->getRigidBody(0));
					}

					PhysicsHandle::physCom->destroyPhysicsWorld(physSim);

					if (editWorld)
					{
						int max = Physics::PhysicsHandle::editWorld->getNbRigidBodies();
						for (int i = 0; i < max; i++)
						{
							Physics::PhysicsHandle::editWorld->destroyRigidBody(Physics::PhysicsHandle::editWorld->getRigidBody(0));
						}

						Physics::PhysicsHandle::physCom->destroyPhysicsWorld(Physics::PhysicsHandle::editWorld);
					}
				}

				inline static void Terminate()
				{
					int max = editWorld->getNbRigidBodies();
					for (int i = 0; i < max; i++)
					{
						editWorld->destroyRigidBody(editWorld->getRigidBody(0));
					}

					PhysicsHandle().physCom->destroyPhysicsWorld(editWorld);
				}

		};
	}
}

#endif // !__PHYSICS_HANDLE_HPP__
