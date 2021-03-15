#ifndef __PHYSICS_SIMULATOR_HPP__
#define __PHYSICS_SIMULATOR_HPP__

namespace reactphysics3d
{
	class PhysicsCommon;
	class PhysicsWorld;
}

namespace Cookie
{
	namespace Physics
	{
		class PhysicsSimulator
		{
			private:
				reactphysics3d::PhysicsCommon* physCreator;
				reactphysics3d::PhysicsWorld* worldSim;
				float accumulator = 0.f;

			public:
				float timeStep		= .0f;
				float factor		= 0.f;


				PhysicsSimulator(reactphysics3d::PhysicsCommon& _physCreator);
				~PhysicsSimulator();

				void Update();
		};
	}
}

#endif //__PHYSICS_SIMULATOR_HPP__