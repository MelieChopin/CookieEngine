#include "ParticlesUpdate.hpp"
#include "ParticlesData.hpp"
#include "Time.hpp"

using namespace Cookie::Resources::Particles;

void UpdateVelocity::Update(ParticlesData* p, int index)
{
	p->pos[index] += p->vel[index] * Cookie::Core::DeltaTime();
}

void EnabledGravity::Update(ParticlesData* p, int index)
{
	p->vel[index] += Cookie::Core::Math::Vec3(0, -9.81f * p->mass[index], 0) * Cookie::Core::DeltaTime();
}

void UpdateTime::Update(ParticlesData* p, int index)
{
	p->time[index] -= Cookie::Core::DeltaTime();
}