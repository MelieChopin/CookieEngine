#ifndef __PARTICLESGENERATOR_HPP__
#define __PARTICLESGENERATOR_HPP__

#include <random>
#include "Vec3.hpp"
#include "ParticlesData.hpp"

namespace Cookie
{
	namespace Core::Math
	{
		union Mat4;
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
				Cookie::Core::Math::Mat4* trs;

				virtual void generate(ParticlesData* data, int start, int end) override;

				BoxPositionGenerate() { type = "BoxPositionGen"; }
				BoxPositionGenerate(Cookie::Core::Math::Vec3 _pos, Cookie::Core::Math::Vec3 _sizeBox, Cookie::Core::Math::Mat4* mat) : pos(_pos), sizeBox(_sizeBox), trs(mat) { type = "BoxPositionGen"; }
			};

			class SpherePositionGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 pos; 
				Cookie::Core::Math::Mat4* trs;
				float radius;

				virtual void generate(ParticlesData* data, int start, int end) override;

				SpherePositionGenerate() { type = "CirclePositionGen"; }
				SpherePositionGenerate(Cookie::Core::Math::Vec3 _pos, float radius, Cookie::Core::Math::Mat4* mat) : pos(_pos), trs(mat), radius(radius) { type = "CirclePositionGen"; }
			};

			class ScaleConstGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 scale;
				virtual void generate(ParticlesData* data, int start, int end) override;

				ScaleConstGenerate(Cookie::Core::Math::Vec3 scale = Cookie::Core::Math::Vec3(1, 1, 1)) : scale(scale) { type = "ScaleConstGenerate"; }
				~ScaleConstGenerate() {}
			};


			class ScaleRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 scaleMin;
				Cookie::Core::Math::Vec3 scaleMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ScaleRandGenerate() { type = "ScaleRandGenerate"; }
				ScaleRandGenerate(Cookie::Core::Math::Vec3 scaleMin, Cookie::Core::Math::Vec3 scaleMax) : scaleMin(scaleMin), scaleMax(scaleMax) { type = "ScaleRandGenerate"; }
				~ScaleRandGenerate() {}
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
				VelocityRandGenerate(Cookie::Core::Math::Vec3 velMin, Cookie::Core::Math::Vec3 velMax) : velMin(velMin), velMax(velMax) { type = "VelRandGen"; }
				~VelocityRandGenerate() {}
			};

			class AccelerationConstGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 acc;

				virtual void generate(ParticlesData* data, int start, int end) override;

				AccelerationConstGenerate() { type = "AccConstGen"; }
				AccelerationConstGenerate(Cookie::Core::Math::Vec3 acc) : acc(acc) { type = "AccConstGen"; }
				~AccelerationConstGenerate() {}

			};

			class AccelerationRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 accMin;
				Cookie::Core::Math::Vec3 accMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				AccelerationRandGenerate() { type = "AccRandGen"; }
				AccelerationRandGenerate(Cookie::Core::Math::Vec3 accMin, Cookie::Core::Math::Vec3 accMax) : accMin(accMin), accMax(accMax) { type = "AccRandGen"; }
				~AccelerationRandGenerate() {}
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
				Cookie::Core::Math::Vec4 minCol;
				Cookie::Core::Math::Vec4 maxCol;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ColorRandGenerate() { type = "ColorRandGen"; }
				ColorRandGenerate(Cookie::Core::Math::Vec4 colorMin, Cookie::Core::Math::Vec4 colorMax) : minCol(colorMin), maxCol(colorMax) { type = "ColorRandGen"; }
			};

			class ColorConstGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec4 col;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ColorConstGenerate() { type = "ColorConstGen"; }
				ColorConstGenerate(Cookie::Core::Math::Vec4 color) : col(color) { type = "ColorConstGen"; }
			};
		}
	}
}


#endif // !__PARTICLESGENERATOR_HPP__
