#ifndef __PARTICLESUPADATE_HPP__
#define __PARTICLESUPADATE_HPP__

#include "Vec3.hpp"
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

			class UpdateScale : public ParticlesUpdate
			{
			public:
				UpdateScale() { type = "UpdateScale"; }
				~UpdateScale() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateAlpha : public ParticlesUpdate
			{
			public:
				UpdateAlpha() { type = "UpdateAlpha"; }
				~UpdateAlpha() {}

				virtual void Update(ParticlesData* p) override;
			};

			class EnabledGravity : public ParticlesUpdate
			{
			public:
				EnabledGravity() { type = "EnabledGravity"; }
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
				CollisionWithPlane(Cookie::Core::Math::Vec3 normal = {0, 1, 0}, float distance = 0) : n(normal), dis(distance) { type = "CollisionWithPlane"; }
				~CollisionWithPlane() {}

				float dis;
				Cookie::Core::Math::Vec3 n;

				virtual void Update(ParticlesData* p) override;
			};
		}
	}
}


#endif // !__PARTICLESUPADATE_HPP__
