#include "ParticlesHandler.hpp"

using namespace Cookie::Resources::Particles;

void ParticlesHandler::Update()
{
	for (int i = 0; i < particlesSystems.size(); i++)
		particlesSystems[i].Update();
}