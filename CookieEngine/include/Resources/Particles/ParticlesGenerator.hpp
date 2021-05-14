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
		static Cookie::Core::Math::Vec4 Random(Cookie::Core::Math::Vec3& min, Cookie::Core::Math::Vec3& max);
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
				std::string type;

				ParticlesGenerator() {}
				~ParticlesGenerator() {}
			};

			class BoxPositionGenerate : public ParticlesGenerator
			{
			public :
				Cookie::Core::Math::Vec3 pos;
				Cookie::Core::Math::Vec3 sizeBox;

				virtual void generate(ParticlesData* data, int start, int end) override;

				BoxPositionGenerate() { type = "BoxPositionGen"; }
				BoxPositionGenerate(Cookie::Core::Math::Vec3 _pos, Cookie::Core::Math::Vec3 _sizeBox) : pos(_pos), sizeBox(_sizeBox) { type = "BoxPositionGen"; }
			};

			class CirclePositionGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 pos; 
				float radius;

				virtual void generate(ParticlesData* data, int start, int end) override;

				CirclePositionGenerate() { type = "CirclePositionGen"; }
				CirclePositionGenerate(Cookie::Core::Math::Vec3 _pos, float radius) : pos(_pos), radius(radius) { type = "CirclePositionGen"; }
			};

			class VelocityConstGenerate : public ParticlesGenerator
			{
			public :
				Cookie::Core::Math::Vec3 vel;

				virtual void generate(ParticlesData* data, int start, int end) override;

				VelocityConstGenerate() { type = "VelConstGen"; }
				VelocityConstGenerate(Cookie::Core::Math::Vec3 vel) : vel(vel) { type = "VelConstGen"; }
				~VelocityConstGenerate() {}

			};

			class VelocityRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 velMin;
				Cookie::Core::Math::Vec3 velMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				VelocityRandGenerate() { type = "VelRandGen"; }
				VelocityRandGenerate(Cookie::Core::Math::Vec3 velMin, Cookie::Core::Math::Vec3 velMax) : velMin(velMin), velMax() { type = "VelRandGen"; }
				~VelocityRandGenerate() {}
			};

			class MassConstGenerate : public ParticlesGenerator
			{
			public:
				float mass;
				virtual void generate(ParticlesData* data, int start, int end) override;

				MassConstGenerate() { type = "MassConstGen"; }
				MassConstGenerate(float mass) : mass(mass) { type = "MassConstGen"; }
			};

			class TimeConstGenerate : public ParticlesGenerator
			{
			public:
				float time;
				virtual void generate(ParticlesData* data, int start, int end) override;

				TimeConstGenerate() { type = "TimeConstGen"; }
				TimeConstGenerate(float time) : time(time) { type = "TimeConstGen"; }
			};

			class TimeRandGenerate : public ParticlesGenerator
			{
			public:
				float timeMin;
				float timeMax;
				virtual void generate(ParticlesData* data, int start, int end) override;

				TimeRandGenerate() { type = "TimeRandGen"; }
				TimeRandGenerate(float timeMin, float timeMax) : timeMin(timeMin), timeMax(timeMax) { type = "TimeRandGen"; }
			};


			class ColorRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 minCol;
				Cookie::Core::Math::Vec3 maxCol;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ColorRandGenerate() { type = "ColorRandGen"; }
				ColorRandGenerate(Cookie::Core::Math::Vec3 colorMin, Cookie::Core::Math::Vec3 colorMax) : minCol(colorMin), maxCol(colorMax) { type = "ColorRandGen"; }
			};
		}
	}
}


#endif // !__PARTICLESGENERATOR_HPP__
