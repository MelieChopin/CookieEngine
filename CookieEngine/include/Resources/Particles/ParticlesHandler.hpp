#ifndef __PARTICLESHANDLER_HPP__

#include "ParticlesSystem.hpp"

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

				void Update();
			};
		}
	}
}

#endif // !__PARTICLESHANDLER_HPP__
