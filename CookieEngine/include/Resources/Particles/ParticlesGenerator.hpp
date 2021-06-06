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

			enum TYPEGEN
			{
				POINTPOSITIONGEN,
				BOXPOSITIONGEN,
				CIRCLEPOSITIONGEN,
				SCALECONSTGEN,
				SCALERANDGEN,
				ROTATERANDGEN,
				VELCONSTGEN,
				VELRANDGEN,
				MASSCONSTGEN,
				TIMECONSTGEN,
				TIMERANDGEN,
				COLORCONSTGEN,
				COLORRANDGEN,
				INITVELWITHPOINT
			};

			class ParticlesGenerator
			{
			public :
				virtual void generate(ParticlesData* data, int start, int end) {}
				TYPEGEN type;

				ParticlesGenerator() {}
				virtual ~ParticlesGenerator() {}
			};

			class PointPositionGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 pos;
				Cookie::Core::Math::Mat4* trs;

				virtual void generate(ParticlesData* data, int start, int end) override;

				PointPositionGenerate() { type = TYPEGEN::POINTPOSITIONGEN; }
				PointPositionGenerate(Cookie::Core::Math::Vec3 _pos, Cookie::Core::Math::Mat4* mat) : pos(_pos), trs(mat) { type = TYPEGEN::POINTPOSITIONGEN; }
				~PointPositionGenerate()override {}
			};

			class BoxPositionGenerate : public ParticlesGenerator
			{
			public :
				Cookie::Core::Math::Vec3 pos;
				Cookie::Core::Math::Vec3 sizeBox;
				Cookie::Core::Math::Mat4* trs;

				virtual void generate(ParticlesData* data, int start, int end) override;

				BoxPositionGenerate() { type = TYPEGEN::BOXPOSITIONGEN; }
				BoxPositionGenerate(Cookie::Core::Math::Vec3 _pos, Cookie::Core::Math::Vec3 _sizeBox, Cookie::Core::Math::Mat4* mat) : pos(_pos), sizeBox(_sizeBox), trs(mat) { type = TYPEGEN::BOXPOSITIONGEN; }
				~BoxPositionGenerate() override {}
			};

			class SpherePositionGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 pos; 
				Cookie::Core::Math::Mat4* trs;
				float radius = 2;

				virtual void generate(ParticlesData* data, int start, int end) override;

				SpherePositionGenerate() { type = TYPEGEN::CIRCLEPOSITIONGEN; }
				SpherePositionGenerate(Cookie::Core::Math::Vec3 _pos, float radius, Cookie::Core::Math::Mat4* mat) : pos(_pos), trs(mat), radius(radius) { type = TYPEGEN::CIRCLEPOSITIONGEN; }
				~SpherePositionGenerate()override {}
			};

			class ScaleConstGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 scale;
				virtual void generate(ParticlesData* data, int start, int end) override;

				ScaleConstGenerate() { type = TYPEGEN::SCALECONSTGEN; }
				ScaleConstGenerate(Cookie::Core::Math::Vec3 scale) : scale(scale) { type = TYPEGEN::SCALECONSTGEN; }
				~ScaleConstGenerate()override {}
			};


			class ScaleRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 scaleMin;
				Cookie::Core::Math::Vec3 scaleMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ScaleRandGenerate() { type = TYPEGEN::SCALERANDGEN; }
				ScaleRandGenerate(Cookie::Core::Math::Vec3 scaleMin, Cookie::Core::Math::Vec3 scaleMax) : scaleMin(scaleMin), scaleMax(scaleMax) { type = TYPEGEN::SCALERANDGEN; }
				~ScaleRandGenerate()override {}
			};

			class RotateRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 rotMin;
				Cookie::Core::Math::Vec3 rotMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				RotateRandGenerate() { type = TYPEGEN::ROTATERANDGEN; }
				RotateRandGenerate(Cookie::Core::Math::Vec3 rotateMin, Cookie::Core::Math::Vec3 rotateMax) : rotMin(rotateMin), rotMax(rotateMax) { type = TYPEGEN::ROTATERANDGEN; }
				~RotateRandGenerate()override {}
			};

			class VelocityConstGenerate : public ParticlesGenerator
			{
			public :
				Cookie::Core::Math::Vec3 vel;

				virtual void generate(ParticlesData* data, int start, int end) override;

				VelocityConstGenerate() { type = TYPEGEN::VELCONSTGEN; }
				VelocityConstGenerate(Cookie::Core::Math::Vec3 vel) : vel(vel) { type = TYPEGEN::VELCONSTGEN; }
				~VelocityConstGenerate()override {}

			};

			class VelocityRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 velMin;
				Cookie::Core::Math::Vec3 velMax;

				virtual void generate(ParticlesData* data, int start, int end) override;

				VelocityRandGenerate() { type = TYPEGEN::VELRANDGEN; }
				VelocityRandGenerate(Cookie::Core::Math::Vec3 velMin, Cookie::Core::Math::Vec3 velMax) : velMin(velMin), velMax(velMax) { type = TYPEGEN::VELRANDGEN; }
				~VelocityRandGenerate()override {}
			};

			class MassConstGenerate : public ParticlesGenerator
			{
			public:
				float mass = 1;
				virtual void generate(ParticlesData* data, int start, int end) override;

				MassConstGenerate() { type = TYPEGEN::MASSCONSTGEN; }
				MassConstGenerate(float mass) : mass(mass) { type = TYPEGEN::MASSCONSTGEN; }
				~MassConstGenerate()override {}
			};

			class TimeConstGenerate : public ParticlesGenerator
			{
			public:
				float time = 1;
				virtual void generate(ParticlesData* data, int start, int end) override;

				TimeConstGenerate() { type = TYPEGEN::TIMECONSTGEN; }
				TimeConstGenerate(float time) : time(time) { type = TYPEGEN::TIMECONSTGEN; }
				~TimeConstGenerate()override {}
			};

			class TimeRandGenerate : public ParticlesGenerator
			{
			public:
				float timeMin = 1;
				float timeMax = 2;
				virtual void generate(ParticlesData* data, int start, int end) override;

				TimeRandGenerate() { type = TYPEGEN::TIMERANDGEN; }
				TimeRandGenerate(float timeMin, float timeMax) : timeMin(timeMin), timeMax(timeMax) { type = TYPEGEN::TIMERANDGEN; }
				~TimeRandGenerate()override {}
			};

			class ColorRandGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec4 minCol;
				Cookie::Core::Math::Vec4 maxCol;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ColorRandGenerate() { type = TYPEGEN::COLORRANDGEN; }
				ColorRandGenerate(Cookie::Core::Math::Vec4 colorMin, Cookie::Core::Math::Vec4 colorMax) : minCol(colorMin), maxCol(colorMax) { type = TYPEGEN::COLORRANDGEN; }
				~ColorRandGenerate()override {}
			};

			class ColorConstGenerate : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec4 col;

				virtual void generate(ParticlesData* data, int start, int end) override;

				ColorConstGenerate() { type = TYPEGEN::COLORCONSTGEN; }
				ColorConstGenerate(Cookie::Core::Math::Vec4 color) : col(color) { type = TYPEGEN::COLORCONSTGEN; }
				~ColorConstGenerate()override {}
			};

			class InitVelocityWithPoint : public ParticlesGenerator
			{
			public:
				Cookie::Core::Math::Vec3 endPoint;

				virtual void generate(ParticlesData* data, int start, int end) override;

				InitVelocityWithPoint() { type = TYPEGEN::INITVELWITHPOINT; }
				InitVelocityWithPoint(Cookie::Core::Math::Vec3 point) : endPoint(point) { type = TYPEGEN::INITVELWITHPOINT; }
				~InitVelocityWithPoint()override {}

			};
		}
	}
}


#endif // !__PARTICLESGENERATOR_HPP__
