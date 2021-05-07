#include "ParticlesUpdate.hpp"
#include "ParticlesData.hpp"
#include "Time.hpp"
#include "Mat4.hpp"

using namespace Cookie::Resources::Particles;

void UpdateVelocity::Update(ParticlesData* p, int index)
{
	Cookie::Core::Math::Vec3 temp = p->vel[index] * Cookie::Core::DeltaTime();
	p->trs[index].c[0].w += temp.x;
	p->trs[index].c[1].w += temp.y;
	p->trs[index].c[2].w += temp.z;
}

void UpdateScale::Update(ParticlesData* p, int index)
{
	p->trs[index].c[0].x -= p->trs[index].c[0].x <= 0 ? 0: 0.005f;
	p->trs[index].c[1].y -= p->trs[index].c[1].y <= 0 ? 0: 0.005f;
	p->trs[index].c[2].z -= p->trs[index].c[2].z <= 0 ? 0: 0.005f;
}

void EnabledGravity::Update(ParticlesData* p, int index)
{
	p->vel[index] += Cookie::Core::Math::Vec3(0, -9.81f * p->mass[index], 0) * Cookie::Core::DeltaTime();
}

void UpdateTime::Update(ParticlesData* p, int index)
{
	p->time[index] -= Cookie::Core::DeltaTime();
}