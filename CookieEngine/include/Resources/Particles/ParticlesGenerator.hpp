#ifndef __PARTICLESGENERATOR_HPP__
#define __PARTICLESGENERATOR_HPP__

#include <random>
#include "Vec3.hpp"
#include "ParticlesData.hpp"

namespace Cookie
{
	namespace Core::Math
	{
		//in inline ?
		static Cookie::Core::Math::Vec3 Random(Cookie::Core::Math::Vec3& min, Cookie::Core::Math::Vec3& max);
	}
	
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesData;

			class ParticlesGenerator
			{
			public :
				virtual void generate(ParticlesData* data, int start, int end) = 0;

				ParticlesGenerator() {}
				~ParticlesGenerator() {}
			};

			class BoxPositionGenerate : public ParticlesGenerator
			{
			public :
				Cookie::Core::Math::Vec3 pos;
				Cookie::Core::Math::Vec3 sizeBox;

				virtual void generate(ParticlesData* data, int start, int end) override;

				BoxPositionGenerate() {}
				BoxPositionGenerate(Cookie::Core::Math::Vec3 _pos, Cookie::Core::Math::Vec3 _sizeBox) : pos(_pos), sizeBox(_sizeBox) {}
			};

			class CirclePositionGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 pos; 
				float radius;

				virtual void generate(ParticlesData* data, int start, int end) override;

				CirclePositionGenerate() {}
				CirclePositionGenerate(Cookie::Core::Math::Vec3 _pos, float radius) : pos(_pos), radius(radius) {}
			};

			class VelocityConstGenerate : public ParticlesGenerator
			{
			public :
				Cookie::Core::Math::Vec3 vel;

				virtual void generate(ParticlesData* data, int start, int end) override;

				VelocityConstGenerate() {}
				VelocityConstGenerate(Cookie::Core::Math::Vec3 vel) : vel(vel) {}
				~VelocityConstGenerate() {}

			};

			class VelocityRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 velMin;
				Cookie::Core::Math::Vec3 velMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				VelocityRandGenerate() {}
				VelocityRandGenerate(Cookie::Core::Math::Vec3 velMin, Cookie::Core::Math::Vec3 velMax) : velMin(velMin), velMax() {}
				~VelocityRandGenerate() {}
			};

			class MassConstGenerate : public ParticlesGenerator
			{
			public:
				float mass;
				virtual void generate(ParticlesData* data, int start, int end) override;

				MassConstGenerate() {}
				MassConstGenerate(float mass) : mass(mass) {}
			};

			class TimeGenerate : public ParticlesGenerator
			{
			public:
				float time;
				virtual void generate(ParticlesData* data, int start, int end) override;

				TimeGenerate() {}
				TimeGenerate(float time) : time(time) {}
			};

			class TimeRandGenerate : public ParticlesGenerator
			{
			public:
				float timeMin;
				float timeMax;
				virtual void generate(ParticlesData* data, int start, int end) override;

				TimeRandGenerate() {}
				TimeRandGenerate(float timeMin, float timeMax) : timeMin(timeMin), timeMax(timeMax) {}
			};
		}
	}
}


#endif // !__PARTICLESGENERATOR_HPP__
