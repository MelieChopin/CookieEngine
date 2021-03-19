#include <reactphysics3d/reactphysics3d.h>
#include "imgui.h"
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
	int max = worldSim->getNbRigidBodies();
	for (int i = 0; i < max; i++)
	{
		worldSim->destroyRigidBody(worldSim->getRigidBody(0));
	}
	physCreator->destroyPhysicsWorld(worldSim);
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