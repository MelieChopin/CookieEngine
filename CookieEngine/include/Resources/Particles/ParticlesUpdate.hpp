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

			class ParticlesUpdate
			{
			public :
				virtual void Update(ParticlesData* p) = 0;
				std::string type;

				ParticlesUpdate() {}
				~ParticlesUpdate() {}
			};

			class UpdateVelocity : public ParticlesUpdate
			{
			public :
				UpdateVelocity() { type = "UpdateVel"; }
				~UpdateVelocity() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateAcc : public ParticlesUpdate
			{
			public:
				UpdateAcc() { type = "UpdateAcc"; }
				~UpdateAcc() {}

				virtual void Update(ParticlesData* p) override;
			};

			class SlowDown : public ParticlesUpdate
			{
			public:
				float coeff;
				SlowDown(float coeff = 0.2f): coeff(coeff) { type = "SlowDown"; }
				~SlowDown() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateScale : public ParticlesUpdate
			{
			public:
				Cookie::Core::Math::Vec3 scaleEnd;
				UpdateScale(Cookie::Core::Math::Vec3 scale = Cookie::Core::Math::Vec3(0, 0, 0)) : scaleEnd(scale) { type = "UpdateScale"; }
				~UpdateScale() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateAlpha : public ParticlesUpdate
			{
			public:
				float alphaEnd;
				UpdateAlpha(float alpha = 0): alphaEnd(alpha) { type = "UpdateAlpha"; }
				~UpdateAlpha() {}

				virtual void Update(ParticlesData* p) override;
			};

			class ColorOverLife : public ParticlesUpdate
			{
			public:
				Cookie::Core::Math::Vec4 colorEnd;
				ColorOverLife(Cookie::Core::Math::Vec4 color = Cookie::Core::Math::Vec4(1, 1, 1, 1)) : colorEnd(color) { type = "ColorOverLife"; }
				~ColorOverLife() {}

				virtual void Update(ParticlesData* p) override;
			};

			class EnabledGravity : public ParticlesUpdate
			{
			public:
				float gravity;
				EnabledGravity(float gravity = -9.81f) : gravity(gravity) { type = "EnabledGravity"; }
				~EnabledGravity() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateTime : public ParticlesUpdate
			{
			public :
				UpdateTime() { type = "UpdateTime"; }
				~UpdateTime() {}

				virtual void Update(ParticlesData* p) override;
			};

			class Loop : public ParticlesUpdate
			{
			public:
				Loop(std::vector<ParticlesGenerator*>& _particlesGen) : particlesGen(_particlesGen) { type = "Loop"; }
				~Loop() {}
				std::vector<ParticlesGenerator*>& particlesGen;

				virtual void Update(ParticlesData* p) override;
			};

			class CollisionWithPlane : public ParticlesUpdate
			{
			public:
				CollisionWithPlane(Cookie::Core::Math::Vec3 normal = {0, 1, 0}, float distance = 0) : dis(distance), n(normal) { type = "CollisionWithPlane"; }
				~CollisionWithPlane() {}

				float dis;
				Cookie::Core::Math::Vec3 n;

				virtual void Update(ParticlesData* p) override;
			};
		}
	}
}


#endif // !__PARTICLESUPADATE_HPP__
