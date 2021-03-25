#ifndef __PHYSICS_HANDLE_HPP__
#define __PHYSICS_HANDLE_HPP__

#include "Physics/PhysicsSimulator.hpp"

namespace Cookie
{
	namespace Physics
	{
		class PhysicsHandle
		{
			public:
				inline static const std::unique_ptr<rp3d::PhysicsCommon>	physCom{std::make_unique<rp3d::PhysicsCommon>()};
				inline static rp3d::PhysicsWorld*							physSim{nullptr};

		};
	}
}

#endif // !__PHYSICS_HANDLE_HPP__
