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
				std::vector<ParticlesGenerator*> generators;
				std::vector<ParticlesUpdate*>  updates;

				ParticlesEmitter() {}
				~ParticlesEmitter() {}
			};
		}
	}
}

#endif // !__PARTICLESEMITER_HPP__
