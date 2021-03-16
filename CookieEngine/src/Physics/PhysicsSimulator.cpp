#include <reactphysics3d/reactphysics3d.h>
#include "Core/Time.hpp"
#include "Physics/PhysicsSimulator.hpp"

using namespace Cookie::Physics;

constexpr float initTimeStep = 1.0f / 60.0f;

/*================ CONSTRUCTORS/DESTRUCTORS ================*/

PhysicsSimulator::PhysicsSimulator(std::shared_ptr<reactphysics3d::PhysicsCommon> _physCreator)
	:physCreator{ _physCreator }, worldSim{ _physCreator->createPhysicsWorld() }, timeStep{initTimeStep}
{
}

PhysicsSimulator::~PhysicsSimulator()
{
	physCreator->destroyPhysicsWorld(worldSim);
}

/*================ REALTIME METHODS ================*/

void PhysicsSimulator::Update()
{
	accumulator += Core::deltaTime;

	while (accumulator >= timeStep)
	{
		worldSim->update(timeStep);

		accumulator -= timeStep;
	}

	factor = accumulator / timeStep;
}