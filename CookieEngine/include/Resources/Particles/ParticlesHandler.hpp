#ifndef __PARTICLESHANDLER_HPP__

#include "ParticlesSystem.hpp"
#include "DrawDataHandler.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesHandler
			{
			public:
				std::list<ParticlesSystem> particlesSystems;
				unsigned int living = 0;

				Render::Frustrum frustrum;
				Cookie::Render::ParticlesPass shader;

				ParticlesHandler() 
				{ 
					particlesSystems.resize(500);
				}

				void Update();
				void Draw(const Render::Camera& cam);
				void CreateParticlesWithPrefab(const Cookie::Core::Math::Vec3& pos, ParticlesPrefab* prefab);
			};
		}
	}
}

#endif // !__PARTICLESHANDLER_HPP__
