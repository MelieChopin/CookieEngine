#include "ParticlesGenerator.hpp"
#include "ParticlesData.hpp"
#include "Mat4.hpp"

Cookie::Core::Math::Vec4 Cookie::Core::Math::Random(Cookie::Core::Math::Vec4& min, Cookie::Core::Math::Vec4& max)
{
	return Cookie::Core::Math::Vec4(float(rand()) / float((RAND_MAX)) * (max.x - min.x) + min.x,
									float(rand()) / float((RAND_MAX)) * (max.y - min.y) + min.y,
									float(rand()) / float((RAND_MAX)) * (max.z - min.z) + min.z,
									float(rand()) / float((RAND_MAX)) * (max.w - min.w) + min.w);
}

Cookie::Core::Math::Vec3 Cookie::Core::Math::Random(Cookie::Core::Math::Vec3& min, Cookie::Core::Math::Vec3& max)
{
	return Cookie::Core::Math::Vec3(float(rand()) / float((RAND_MAX)) * (max.x - min.x) + min.x,
									float(rand()) / float((RAND_MAX)) * (max.y - min.y) + min.y,
									float(rand()) / float((RAND_MAX)) * (max.z - min.z) + min.z);
}

float Random(float min, float max)
{
	return float(rand()) / float((RAND_MAX)) * (max - min + 1) + min;
}

using namespace Cookie::Resources::Particles;

void IsBillboard::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->isBillboard[i] = true;
}

void NotBillboard::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->isBillboard[i] = false;
}

void BoxPositionGenerate::generate(ParticlesData* data, int start, int end)
{
	Cookie::Core::Math::Vec3 posMin( pos.x - sizeBox.x,
									 pos.y - sizeBox.y,
									 pos.z - sizeBox.z );

	Cookie::Core::Math::Vec3 posMax( pos.x + sizeBox.x,
									 pos.y + sizeBox.y,
									 pos.z + sizeBox.z);

	for (int i = start; i < end; i++)
	{
		Cookie::Core::Math::Vec3 temp = Cookie::Core::Math::Random(posMin, posMax);
		data->trs[i].c[0].w = temp.x;
		data->trs[i].c[1].w = temp.y;
		data->trs[i].c[2].w = temp.z;
		data->trs[i] = data->trs[i] * (*trs);
	}
}

void SpherePositionGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		float angle = Random(0.f, 360.f);
		data->trs[i].c[0].w = radius * cos(angle);
		data->trs[i].c[1].w = radius * cos(angle);
		data->trs[i].c[2].w = radius * sin(angle);	
		data->trs[i] = data->trs[i] * (*trs);
	}
}

void VelocityConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->vel[i] = vel;
}

void VelocityRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->vel[i] = Cookie::Core::Math::Random(velMin, velMax);
}

void MassConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->mass[i] = mass;
}

void TimeConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->time[i] = time;
}

void TimeRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->time[i] = Random(timeMin, timeMax);
}

void ColorRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->col[i] = Cookie::Core::Math::Random(minCol, maxCol);
}
