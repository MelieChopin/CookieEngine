#include "ParticlesUpdate.hpp"
#include "ParticlesData.hpp"
#include "Time.hpp"
#include "Mat4.hpp"

using namespace Cookie::Resources::Particles;

void UpdateVelocity::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		Cookie::Core::Math::Vec3 temp = p->vel[i] * Cookie::Core::DeltaTime();
		p->trs[i].c[0].w += temp.x;
		p->trs[i].c[1].w += temp.y;
		p->trs[i].c[2].w += temp.z;
	}
}

void UpdateScale::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		p->trs[i].c[0].x -= p->trs[i].c[0].x <= 0 ? 0 : 0.005f;
		p->trs[i].c[1].y -= p->trs[i].c[1].y <= 0 ? 0 : 0.005f;
		p->trs[i].c[2].z -= p->trs[i].c[2].z <= 0 ? 0 : 0.005f;
	}
}

void EnabledGravity::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
		p->vel[i] += Cookie::Core::Math::Vec3(0, -9.81f * p->mass[i], 0) * Cookie::Core::DeltaTime();
}

void UpdateTime::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		p->time[i] -= Cookie::Core::DeltaTime();
		if (p->time[i] < 0)
			p->kill(i);
	}
}

void Loop::Update(ParticlesData* p)
{
	if (p->countAlive < p->countFrame)
	{
		float countAlive = p->countAlive;
		float countFrame = p->countFrame;

		p->wake(countAlive, countFrame);

		for (int i = 0; i < particlesGen.size(); i++)
			particlesGen[i]->generate(p, countAlive, countFrame + 1);
	}
}