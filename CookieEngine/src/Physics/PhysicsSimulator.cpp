#include "Core/Time.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Physics/PhysicsSimulator.hpp"

using namespace Cookie::Physics;

constexpr float initTimeStep = 1.0f / 60.0f;

/*================ CONSTRUCTORS/DESTRUCTORS ================*/

PhysicsSimulator::PhysicsSimulator()
	:worldSim{ Physics::PhysicsHandle::physSim}, timeStep{initTimeStep}
{
}

PhysicsSimulator::~PhysicsSimulator()
{
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