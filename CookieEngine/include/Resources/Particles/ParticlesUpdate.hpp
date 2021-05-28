#ifndef __PARTICLESUPADATE_HPP__
#define __PARTICLESUPADATE_HPP__

#include "Vec3.hpp"
#include "Vec4.hpp"
#include <vector>

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesData;
			class ParticlesGenerator;

			enum TYPEUP
			{
				UPDATEVEL,
				UPDATESCALE,
				UPDATEALPHA,
				COLOROVERLIFE,
				ENABLEGRAVITY,
				UPDATETIME,
				LOOP,
				COLLISIONWITHPLANE,
				CREATPARTICLES
			};

			class ParticlesUpdate
			{
			public :
				virtual void Update(ParticlesData* p) = 0;
				TYPEUP type;

				ParticlesUpdate() {}
				~ParticlesUpdate() {}
			};

			class UpdateVelocity : public ParticlesUpdate
			{
			public :
				UpdateVelocity() { type = TYPEUP::UPDATEVEL; }
				~UpdateVelocity() {}

				virtual void Update(ParticlesData* p) override;
			};


			class UpdateScale : public ParticlesUpdate
			{
			public:
				Cookie::Core::Math::Vec3 scaleEnd;
				UpdateScale(Cookie::Core::Math::Vec3 scale = Cookie::Core::Math::Vec3(0, 0, 0)) : scaleEnd(scale) { type = TYPEUP::UPDATESCALE; }
				~UpdateScale() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateAlpha : public ParticlesUpdate
			{
			public:
				float alphaEnd;
				UpdateAlpha(float alpha = 0): alphaEnd(alpha) { type = TYPEUP::UPDATEALPHA; }
				~UpdateAlpha() {}

				virtual void Update(ParticlesData* p) override;
			};

			class ColorOverLife : public ParticlesUpdate
			{
			public:
				Cookie::Core::Math::Vec4 colorEnd;
				ColorOverLife(Cookie::Core::Math::Vec4 color = Cookie::Core::Math::Vec4(1, 1, 1, 1)) : colorEnd(color) { type = TYPEUP::COLOROVERLIFE; }
				~ColorOverLife() {}

				virtual void Update(ParticlesData* p) override;
			};

			class EnabledGravity : public ParticlesUpdate
			{
			public:
				float gravity;
				EnabledGravity(float gravity = -9.81f) : gravity(gravity) { type = TYPEUP::ENABLEGRAVITY; }
				~EnabledGravity() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateTime : public ParticlesUpdate
			{
			public :
				UpdateTime() { type = TYPEUP::UPDATETIME; }
				~UpdateTime() {}

				virtual void Update(ParticlesData* p) override;
			};

			class Loop : public ParticlesUpdate
			{
			public:
				Loop(std::vector<ParticlesGenerator*>& _particlesGen) : particlesGen(&_particlesGen) { type = TYPEUP::LOOP; }
				~Loop() {}
				std::vector<ParticlesGenerator*>* particlesGen;

				virtual void Update(ParticlesData* p) override;
			};

			class CollisionWithPlane : public ParticlesUpdate
			{
			public:
				CollisionWithPlane(Cookie::Core::Math::Vec3 normal = {0, 1, 0}, float distance = 0) : dis(distance), n(normal) { type = TYPEUP::COLLISIONWITHPLANE; }
				~CollisionWithPlane() {}

				float dis;
				Cookie::Core::Math::Vec3 n;

				virtual void Update(ParticlesData* p) override;
			};


			class CreateParticles : public ParticlesUpdate
			{
			public:
				ParticlesData& data;

				virtual void Update(ParticlesData* p) override;

				CreateParticles(ParticlesData& data) : data(data) { type = TYPEUP::CREATPARTICLES; }
			};
		}
	}
}


#endif // !__PARTICLESUPADATE_HPP__
