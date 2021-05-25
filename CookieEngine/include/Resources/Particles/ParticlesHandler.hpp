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
				std::vector < Cookie::Resources::Particles::ParticlesSystem > particlesSystems;

				Render::Frustrum frustrum;

				void Update();
				void Draw(const Render::Camera& cam);
			};
		}
	}
}

#endif // !__PARTICLESHANDLER_HPP__
