#include "ParticlesHandler.hpp"
#include "Camera.hpp"
#include "DrawDataHandler.hpp"
#include "ResourcesManager.hpp"

using namespace Cookie::Resources::Particles;

std::list<ParticlesSystem> ParticlesHandler::particlesSystems;
unsigned int ParticlesHandler::living;
std::unordered_map<std::string, std::unique_ptr<ParticlesPrefab>>* ParticlesHandler::particlesPrefab;
Cookie::Render::ParticlesPass ParticlesHandler::shader;

void ParticlesHandler::Update()
{
	for (std::list<ParticlesSystem>::iterator particles = particlesSystems.begin(); particles != particlesSystems.end(); particles++)
	{
		(*particles).Update();
		if ((*particles).needToBeRemoved)
		{
			particles = particlesSystems.erase(particles);
			particlesSystems.push_back(ParticlesSystem());
			living--;
		}
	}
		
}

void ParticlesHandler::Draw(const Render::Camera& cam)
{
	frustrum.MakeFrustrum(cam);
	for (std::list<ParticlesSystem>::iterator particles = particlesSystems.begin(); particles != particlesSystems.end(); particles++)
		(*particles).Draw(cam, frustrum);
}

void ParticlesHandler::CreateParticlesWithPrefab
(const Cookie::Core::Math::Vec3& pos, const std::string& namePrefab, const Cookie::Core::Math::Vec3& posSpawnEnd)
{
	if (living + 1 >= particlesSystems.size() || particlesPrefab->find(namePrefab) == particlesPrefab->end())
		return;
	ParticlesSystem& particles = *std::next(particlesSystems.begin(), living);

	ParticlesPrefab* prefab = (*particlesPrefab)[namePrefab].get();

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
			else if (name == "Loop")
			{
				Loop* loop = new Loop(particles.particlesEmiter[i].generators);
				particles.particlesEmiter[i].updates.push_back(loop);
			}
			else if (name == "BoxPositionGen")
			{
				BoxPositionGenerate* box = new BoxPositionGenerate();
				box->pos = prefab->emit[i][j].data[0];
				box->sizeBox = prefab->emit[i][j].data[1];
				box->trs = &particles.trs;
				particles.particlesEmiter[i].generators.push_back(box);
			}
			else if (name == "CirclePositionGen")
			{
				SpherePositionGenerate* sphere = new SpherePositionGenerate();
				sphere->pos = prefab->emit[i][j].data[0];
				sphere->radius = prefab->emit[i][j].data[1].x;
				sphere->trs = &particles.trs;
				particles.particlesEmiter[i].generators.push_back(sphere);
			}
			else if (name == "CreateParticles")
			{
				if (particles.data.size() > prefab->emit[i][j].data[0].x)
				{
					CreateParticlesFollowing* create = new CreateParticlesFollowing(particles.data[prefab->emit[i][j].data[0].x]);
					particles.data[prefab->emit[i][j].data[0].x].canRemoved = false;
					create->coeffScale = prefab->emit[i][j].data[1].x;
					create->coeffPos = prefab->emit[i][j].data[1].y;
					create->time = prefab->emit[i][j].data[1].z;
					particles.particlesEmiter[i].updates.push_back(create);
				}
			}
			else if (name == "CollisionWithPlane")
			{
				CollisionWithPlane* plane = new CollisionWithPlane();
				plane->dis = prefab->emit[i][j].data[1].x;
				plane->n = prefab->emit[i][j].data[0];
				plane->namePrefab = prefab->emit[i][j].nameData;
				particles.particlesEmiter[i].updates.push_back(plane);
			}
			else if (name == "Shadow")
			{
				if (particles.data.size() > prefab->emit[i][j].data[0].x)
				{
					Shadow* shadow = new Shadow(particles.data[prefab->emit[i][j].data[0].x]);
					particles.data[prefab->emit[i][j].data[0].x].canRemoved = false;
					shadow->time = prefab->emit[i][j].data[0].y;
					particles.particlesEmiter[i].updates.push_back(shadow);
				}
			}
			else if (name == "SpawnEnd")
			{
				SpawnEnd* plane = new SpawnEnd();
				plane->namePrefab = prefab->emit[i][j].nameData;
				plane->posSpawn = posSpawnEnd;
				particles.particlesEmiter[i].updates.push_back(plane);
			}
			else if (name == "InitVelWithPoint")
			{
				InitVelocityWithPoint* vel = new InitVelocityWithPoint();
				vel->endPoint = posSpawnEnd;
				particles.particlesEmiter[i].generators.push_back(vel);
			}
		}
	}

	particles.needToBeRemoved = false;
	particles.generate();
	living++;
}