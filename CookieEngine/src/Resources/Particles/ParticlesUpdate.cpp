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

void UpdateAlpha::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		p->col[i].a = p->col[i].a <= 0 ? 0 : p->col[i].a - 0.009f;
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

void CollisionWithPlane::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		Cookie::Core::Math::Vec3 pos(p->trs[i].c[0].w, p->trs[i].c[1].w, p->trs[i].c[2].w);
		float distance = (pos.Dot(n) + dis) / n.Length();
		
		if (distance <= Cookie::Core::Math::EPSILON && p->isBillboard[i] == true)
		{
			p->isBillboard[i] = false;
			p->vel[i] = Cookie::Core::Math::Vec3(0, 0, 0);
			p->mass[i] = 0;
			p->trs[i] = Cookie::Core::Math::Mat4::RotateX(-Cookie::Core::Math::PI / 2) * p->trs[i];
		}
	}
}