#ifndef __PHYSICS_SIMULATOR_HPP__
#define __PHYSICS_SIMULATOR_HPP__

class reactphysics3d::PhysicsCommon;
class reactphysics3d::PhysicsWorld;

namespace Cookie
{
	namespace Physics
	{
		class PhysicsSimulator
		{
			reactphysics3d::PhysicsCommon	physicObjectCreator;
			reactphysics3d::PhysicsWorld	worldSimulator;
		}
	}
}

#endif //__PHYSICS_SIMULATOR_HPP__