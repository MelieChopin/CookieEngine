#ifndef __PARTICLESUPADATE_HPP__
#define __PARTICLESUPADATE_HPP__

#include "Vec3.hpp"
#include "Vec4.hpp"
#include <vector>

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;

		namespace Particles
		{
			class ParticlesData;
			class ParticlesGenerator;
			class ParticlesHandler;

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
				CREATEPARTICLES,
				SHADOW,
				SPAWNEND
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
				float gravity = -9.81f;
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
				CollisionWithPlane(std::string namePrefab = "Explosion",
					Cookie::Core::Math::Vec3 normal = {0, 1, 0}, float distance = -0.58f)
					: dis(distance), n(normal) { type = TYPEUP::COLLISIONWITHPLANE; }
				~CollisionWithPlane() {}

				float dis = -0.58f;
				Cookie::Core::Math::Vec3 n = Cookie::Core::Math::Vec3(0, 1, 0);
				std::string namePrefab = "Explosion";

				virtual void Update(ParticlesData* p) override;
			};


			class CreateParticlesFollowing : public ParticlesUpdate
			{
			public:
				ParticlesData* data;
				float coeffScale = 1;
				float coeffPos = 3;
				float time = 0.35f;

				virtual void Update(ParticlesData* p) override;

				CreateParticlesFollowing(ParticlesData& data, float time = 0.35f, float coeff = 1, float coeffPos = 3)
					: data(&data), coeffScale(coeff), coeffPos(coeffPos), time(time) { type = TYPEUP::CREATEPARTICLES; }
			};

			class Shadow : public ParticlesUpdate
			{
			public:
				ParticlesData* data;
				float time = 0.35f;

				virtual void Update(ParticlesData* p) override;

				Shadow(ParticlesData& data, float time = 0.35f)
					: data(&data), time(time) {
					type = TYPEUP::CREATEPARTICLES;
				}
			};

			class SpawnEnd : public ParticlesUpdate
			{
			public:
				Cookie::Core::Math::Vec3 posSpawn;
				std::string namePrefab = "Explosion";

				virtual void Update(ParticlesData* p) override;

				SpawnEnd()
				{
					type = TYPEUP::SPAWNEND;
				}
			};
		}
	}
}


#endif // !__PARTICLESUPADATE_HPP__
