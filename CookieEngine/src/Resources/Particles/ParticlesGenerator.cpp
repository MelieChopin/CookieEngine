#include "ParticlesGenerator.hpp"
#include "ParticlesData.hpp"
#include "Mat4.hpp"
#include "Calc.hpp"

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
	{
		Cookie::Core::Math::Vec3 temp = Cookie::Core::Math::Random(posMin, posMax);
		data->data[i].pos.x = temp.x;
		data->data[i].pos.y = temp.y;
		data->data[i].pos.z = temp.z;
		data->data[i].pos = (*trs) * data->data[i].pos;
	}
}

void SpherePositionGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		float angle = Random(0.f, 360.f);
		data->data[i].pos.x =  radius * (Random(0, 10) <= 5 ? -1 : 1) * cos(angle);
		data->data[i].pos.y = radius * (Random(0, 10) <= 5 ? -1 : 1) * cos(angle);
		data->data[i].pos.z = radius * (Random(0, 10) <= 5 ? -1 : 1) *  sin(angle);
		data->data[i].pos = (*trs) * data->data[i].pos;
	}
}

void ScaleConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		data->data[i].scale = scale;
		data->data[i].scaleBegin = scale;
	}
}

void ScaleRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		data->data[i].scale = Cookie::Core::Math::Random(scaleMin, scaleMax);
		data->data[i].scaleBegin = data->data[i].scale;
	}
}

void VelocityConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->data[i].vel = vel;
}

void VelocityRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->data[i].vel = Cookie::Core::Math::Random(velMin, velMax);
}

void AccelerationConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->data[i].acc = acc;
}

void AccelerationRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->data[i].acc = Cookie::Core::Math::Random(accMin, accMax);
}

void MassConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
		data->data[i].mass = mass;
}

void TimeConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		data->data[i].time = time;
		data->data[i].timeMax = time;
	}
}

void TimeRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		data->data[i].time = Random(timeMin, timeMax);
		data->data[i].timeMax = data->data[i].time;
	}
}

void ColorRandGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		data->data[i].col = Cookie::Core::Math::Random(minCol, maxCol);
		data->data[i].colBegin = data->data[i].col;
	}
}

void ColorConstGenerate::generate(ParticlesData* data, int start, int end)
{
	for (int i = start; i < end; i++)
	{
		data->data[i].col = col;
		data->data[i].colBegin = data->data[i].col;
	}
}