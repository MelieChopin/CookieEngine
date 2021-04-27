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
				}

				static void Terminate()
				{
					//physCom.reset();
				}

		};
	}
}

#endif // !__PHYSICS_HANDLE_HPP__
