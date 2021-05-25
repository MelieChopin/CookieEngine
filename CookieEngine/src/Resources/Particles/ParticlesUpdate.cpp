#include "ParticlesUpdate.hpp"
#include "ParticlesData.hpp"
#include "Time.hpp"
#include "Mat4.hpp"

using namespace Cookie::Resources::Particles;

void UpdateVelocity::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
		p->data[i].pos += p->data[i].vel * Cookie::Core::DeltaTime();
}

void UpdateAcc::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
		p->data[i].vel += p->data[i].acc * Cookie::Core::DeltaTime();
}

void UpdateScale::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		p->data[i].scale.x = Cookie::Core::Math::Lerp(scaleEnd.x, p->data[i].scaleBegin.x, p->data[i].time / p->data[i].timeMax);
		p->data[i].scale.y = Cookie::Core::Math::Lerp(scaleEnd.y, p->data[i].scaleBegin.y, p->data[i].time / p->data[i].timeMax);
		p->data[i].scale.z = Cookie::Core::Math::Lerp(scaleEnd.z, p->data[i].scaleBegin.z, p->data[i].time / p->data[i].timeMax);
	}
}

void SlowDown::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
		p->data[i].acc *= coeff;
}

void UpdateAlpha::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
		p->data[i].col.a = Cookie::Core::Math::Lerp(alphaEnd, p->data[i].colBegin.a, p->data[i].time / p->data[i].timeMax);
}

void ColorOverLife::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		p->data[i].col.x = Cookie::Core::Math::Lerp(colorEnd.x, p->data[i].colBegin.x, p->data[i].time / p->data[i].timeMax);
		p->data[i].col.y = Cookie::Core::Math::Lerp(colorEnd.y, p->data[i].colBegin.y, p->data[i].time / p->data[i].timeMax);
		p->data[i].col.z = Cookie::Core::Math::Lerp(colorEnd.z, p->data[i].colBegin.z, p->data[i].time / p->data[i].timeMax);
		p->data[i].col.a = Cookie::Core::Math::Lerp(colorEnd.a, p->data[i].colBegin.a, p->data[i].time / p->data[i].timeMax);
	}
}

void EnabledGravity::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
		p->data[i].vel += Cookie::Core::Math::Vec3(0, gravity * p->data[i].mass, 0) * Cookie::Core::DeltaTime();
}

void UpdateTime::Update(ParticlesData* p)
{
	for (int i = 0; i < p->countAlive; i++)
	{
		p->data[i].time -= Cookie::Core::DeltaTime();
		if (p->data[i].time < 0)
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
		Cookie::Core::Math::Vec3 pos(p->data[i].pos.x, p->data[i].pos.y, p->data[i].pos.z);
		float distance = (pos.Dot(n) + dis) / n.Length();
		
		if (distance <= Cookie::Core::Math::EPSILON && p->data[i].isBillboard == true)
		{
			p->data[i].isBillboard = false;
			p->data[i].vel = Cookie::Core::Math::Vec3(0, 0, 0);
			p->data[i].mass = 0;
			p->data[i].rot = Cookie::Core::Math::Vec3(Cookie::Core::Math::PI / 2, 0, 0);
		}
	}
}