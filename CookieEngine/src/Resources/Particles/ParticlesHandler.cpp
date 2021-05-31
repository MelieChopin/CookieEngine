#include "ParticlesHandler.hpp"
#include "Camera.hpp"
#include "DrawDataHandler.hpp"
#include "ResourcesManager.hpp"

using namespace Cookie::Resources::Particles;

void ParticlesHandler::Update()
{
	for (int i = 0; i < living; i++)
	{
		particlesSystems[i].Update();
		if (particlesSystems[i].needToBeRemoved)
		{
			particlesSystems[i].Swap(particlesSystems[living - 1]);
			living--;
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

void ParticlesHandler::CreateParticlesWithPrefab
(const Cookie::Core::Math::Vec3& pos, ParticlesPrefab* prefab)
{
	if (living >= particlesSystems.size())
		return;
	ParticlesSystem& particles = particlesSystems[living];
	particles.shader = &shader;

	particles.name = prefab->name;
	particles.data.resize(prefab->data.size());
	particles.trs = Cookie::Core::Math::Mat4::Translate(pos);
	for (int i = 0; i < prefab->data.size(); i++)
	{
		particles.data[i].generate(prefab->data[i].size, prefab->data[i].countFrame);
		particles.data[i].wake(0, prefab->data[i].countAlive);
		particles.data[i].mesh = prefab->data[i].mesh;
		particles.data[i].texture = prefab->data[i].texture;
		particles.data[i].countFrame = prefab->data[i].countFrame;
		particles.data[i].countAlive = prefab->data[i].countAlive;
		particles.data[i].SetIsBIllboard(prefab->data[i].isBillboard);
		
	}

	particles.particlesEmiter = prefab->emitter;

	std::string name;
	for (int i = 0; i < prefab->emit.size(); i++)
	{
		for (int j = 0; j < prefab->emit[i].size(); j++)
		{
			name = prefab->emit[i][j].name;
			if (name == "PointPositionGen")
			{
				PointPositionGenerate* point = new PointPositionGenerate();
				point->pos = prefab->emit[i][j].data[0];
				point->trs = &particles.trs;
				particles.particlesEmiter[i].generators.push_back(point);
			}
			else if (name == "CreateParticles")
			{
				CreateParticles* create = new CreateParticles(particles.data[prefab->emit[i][j].data[0].x]);
				create->coeffScale = prefab->emit[i][j].data[1].x;
				create->coeffPos = prefab->emit[i][j].data[1].y;
				create->time = prefab->emit[i][j].data[1].z;
				particles.particlesEmiter[i].updates.push_back(create);
			}
		}
	}

	particles.needToBeRemoved = false;
	particles.generate();
	living++;
}