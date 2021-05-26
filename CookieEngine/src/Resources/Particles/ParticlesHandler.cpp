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
			particlesSystems.erase(particlesSystems.begin() + i);
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