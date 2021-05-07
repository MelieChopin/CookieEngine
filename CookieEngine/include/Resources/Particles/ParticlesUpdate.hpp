#ifndef __PARTICLESUPADATE_HPP__
#define __PARTICLESUPADATE_HPP__

#include "Vec3.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesData;

			class ParticlesUpdate
			{
			public :
				virtual void Update(ParticlesData* p, int index) = 0;

				ParticlesUpdate() {}
				~ParticlesUpdate() {}
			};

			class UpdateVelocity : public ParticlesUpdate
			{
			public :
				UpdateVelocity() {}
				~UpdateVelocity() {}

				virtual void Update(ParticlesData* p, int index) override;
			};

			class UpdateScale : public ParticlesUpdate
			{
			public:
				UpdateScale() {}
				~UpdateScale() {}

				virtual void Update(ParticlesData* p, int index) override;
			};

			class EnabledGravity : public ParticlesUpdate
			{
			public:
				EnabledGravity() {}
				~EnabledGravity() {}

				virtual void Update(ParticlesData* p, int index) override;
			};

			class UpdateTime : public ParticlesUpdate
			{
			public :
				UpdateTime() {}
				~UpdateTime() {}

				virtual void Update(ParticlesData* p, int index) override;
			};
		}
	}
}


#endif // !__PARTICLESUPADATE_HPP__
