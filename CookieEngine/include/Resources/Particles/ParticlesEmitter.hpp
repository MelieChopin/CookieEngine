#ifndef __PARTICLESEMITER_HPP__
#define __PARTICLESEMITER_HPP__

#include <vector>
#include "ParticlesGenerator.hpp"
#include "ParticlesUpdate.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesGenerator;

			class ParticlesEmitter
			{
			public:
				std::vector<std::shared_ptr<ParticlesGenerator>> generators;
				std::vector<std::shared_ptr<ParticlesUpdate>>  updates;

				ParticlesEmitter() {}
				~ParticlesEmitter() {}
			};
		}
	}
}

#endif // !__PARTICLESEMITER_HPP__
