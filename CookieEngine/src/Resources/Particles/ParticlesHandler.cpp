#include "ParticlesHandler.hpp"
#include "Camera.hpp"
#include "DrawDataHandler.hpp"

using namespace Cookie::Resources::Particles;

void ParticlesHandler::Update()
{
	for (int i = 0; i < particlesSystems.size(); i++)
	{
		particlesSystems[i].Update();
		if (particlesSystems[i].needToBeRemoved)
		{
			particlesSystems[i].shader.Clean();
			Cookie::Resources::Particles::ParticlesSystem temp = particlesSystems[i];
			particlesSystems[i] = particlesSystems[particlesSystems.size() - 1];
			particlesSystems[particlesSystems.size() - 1] = temp;
			particlesSystems.erase(particlesSystems.end() - 1);
			i--;
			continue;
		}
	}
}

void ParticlesHandler::Draw(const Render::Camera& cam)
{
	frustrum.MakeFrustrum(cam);
	for (int i = 0; i < particlesSystems.size(); i++)
		particlesSystems[i].Draw(cam, frustrum);	
}