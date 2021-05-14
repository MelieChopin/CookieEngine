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

				ParticlesUpdate() {}
				~ParticlesUpdate() {}
			};

			class UpdateVelocity : public ParticlesUpdate
			{
			public :
				UpdateVelocity() {}
				~UpdateVelocity() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateScale : public ParticlesUpdate
			{
			public:
				UpdateScale() {}
				~UpdateScale() {}

				virtual void Update(ParticlesData* p) override;
			};

			class EnabledGravity : public ParticlesUpdate
			{
			public:
				EnabledGravity() {}
				~EnabledGravity() {}

				virtual void Update(ParticlesData* p) override;
			};

			class UpdateTime : public ParticlesUpdate
			{
			public :
				UpdateTime() {}
				~UpdateTime() {}

				virtual void Update(ParticlesData* p) override;
			};

			class Loop : public ParticlesUpdate
			{
			public:
				Loop(std::vector<ParticlesGenerator*>& _particlesGen) : particlesGen(_particlesGen) {}
				~Loop() {}
				std::vector<ParticlesGenerator*>& particlesGen;

				virtual void Update(ParticlesData* p) override;
			};
		}
	}
}


#endif // !__PARTICLESUPADATE_HPP__
