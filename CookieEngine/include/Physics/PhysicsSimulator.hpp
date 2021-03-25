#ifndef __PHYSICS_SIMULATOR_HPP__
#define __PHYSICS_SIMULATOR_HPP__

#include <reactphysics3d/reactphysics3d.h>

namespace Cookie
{
	namespace Physics
	{
		class PhysicsSimulator
		{
			private:
				float accumulator = 0.f;

			public:
				reactphysics3d::PhysicsWorld* worldSim{ nullptr };

				float timeStep		= .0f;
				float factor		= 0.f;


				PhysicsSimulator();
				~PhysicsSimulator();

				void Update();
		};
	}
}

#endif //__PHYSICS_SIMULATOR_HPP__