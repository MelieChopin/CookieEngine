#include "Core/Time.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Physics/PhysicsSimulator.hpp"

using namespace Cookie::Physics;

constexpr float initTimeStep = 1.0f / 60.0f;

/*================ CONSTRUCTORS/DESTRUCTORS ================*/

PhysicsSimulator::PhysicsSimulator()
	:worldSim{ PhysicsHandle::physCom->createPhysicsWorld() }, timeStep{initTimeStep}
{
	Physics::PhysicsHandle::physSim = worldSim;
}

PhysicsSimulator::~PhysicsSimulator()
{
	int max = worldSim->getNbRigidBodies();
	for (int i = 0; i < max; i++)
	{
		worldSim->destroyRigidBody(worldSim->getRigidBody(0));
	}

	PhysicsHandle::physCom->destroyPhysicsWorld(worldSim);
}

/*================ REALTIME METHODS ================*/

void PhysicsSimulator::Update()
{
	accumulator += Cookie::Core::DeltaTime();

	while (accumulator >= timeStep)
	{
		worldSim->update(timeStep);

		accumulator -= timeStep;
	}

	factor = accumulator / timeStep;
}