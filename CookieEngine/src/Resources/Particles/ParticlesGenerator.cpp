#include "ParticlesGenerator.hpp"
#include "ParticlesData.hpp"


Cookie::Core::Math::Vec3 Cookie::Core::Math::Random(Cookie::Core::Math::Vec3& min, Cookie::Core::Math::Vec3& max)
{
	return Cookie::Core::Math::Vec3(float(rand()) / float((RAND_MAX)) * (max.x - min.x + 1) + min.x,
									float(rand()) / float((RAND_MAX)) * (max.y - min.y + 1) + min.y,
									float(rand()) / float((RAND_MAX)) * (max.z - min.z + 1) + min.z);
}

float Random(float min, float max)
{
	return float(rand()) / float((RAND_MAX)) * (max - min + 1) + min;
}

using namespace Cookie::Resources::Particles;

void BoxPositionGenerate::generate(ParticlesData* data, int start, int end)
{
	Cookie::Core::Math::Vec3 posMin( pos.x - sizeBox.x,
									 pos.y - sizeBox.y,
									 pos.z - sizeBox.z );

	Cookie::Core::Math::Vec3 posMax( pos.x + sizeBox.x,
									 pos.y + sizeBox.y,
									 pos.z + sizeBox.z);

	for (int i = start; i < end; i++)
		data->pos[i] = Cookie::Core::Math::Random(posMin, posMax);
}

void CirclePositionGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		float angle = Random(0.f, 360.f);
		data->pos[i] = Cookie::Core::Math::Vec3(radius * cos(angle), 0, radius * sin(angle));
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

void TimeGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->time[i] = time;
}

void TimeRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->time[i] = Random(timeMin, timeMax);
}
