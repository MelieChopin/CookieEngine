#ifndef __PARTICLESHANDLER_HPP__

#include "ParticlesSystem.hpp"
#include "DrawDataHandler.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			#define MAX_PARTICLESSYSTEMS 500	

			class ParticlesHandler
			{
			public:
				static std::list<ParticlesSystem> particlesSystems;
				static unsigned int living;
				static std::unordered_map<std::string, std::unique_ptr<Particles::ParticlesPrefab>>* particlesPrefab;

				Render::Frustrum frustrum;
				static Cookie::Render::ParticlesPass shader;

				ParticlesHandler()
				{ 
					particlesSystems.resize(MAX_PARTICLESSYSTEMS);
					living = 0;
					shader.InitShader();
				}

				void Update();
				void Draw(const Render::Camera& cam);
				static void CreateParticlesWithPrefab(const Cookie::Core::Math::Vec3& pos, const std::string& name, const Cookie::Core::Math::Vec3& posSpawnEnd = Cookie::Core::Math::Vec3(0, 0, 0));
				static void CreateParticlesWithPrefab(const Cookie::Core::Math::Vec3& pos, Particles::ParticlesPrefab* particlesPrefab, const Cookie::Core::Math::Vec3& posSpawnEnd = Cookie::Core::Math::Vec3(0, 0, 0));
			};
		}
	}
}

#endif // !__PARTICLESHANDLER_HPP__
