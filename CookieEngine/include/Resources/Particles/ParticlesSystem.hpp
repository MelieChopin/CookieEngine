#ifndef __PARTICLESSYSTEM_HPP__
#define __PARTICLESSYSTEM_HPP__

#include "ParticlesEmitter.hpp"
#include "ParticlesData.hpp"
#include "ComponentTransform.hpp"

namespace Cookie
{
	namespace Resources
	{
		namespace Particles
		{
			class ParticlesSystem
			{
			public :
				ParticlesData data;
				ParticlesEmitter particlesEmiter;

				ECS::ComponentTransform trs;

				ParticlesSystem() {}

				ParticlesSystem(int size, int sizeFrame)
				{
					data.generate(size, sizeFrame);
				}
				~ParticlesSystem() {}

				void Update()
				{
					if (data.countAlive < data.countFrame)
					{
						float countAlive = data.countAlive;
						float countFrame = data.countFrame;

						data.wake(countAlive, countFrame);

						for (int i = 0; i < particlesEmiter.generators.size(); i++)
							particlesEmiter.generators[i]->generate(&data, countAlive, countFrame);
					}

					for (int i = 0; i < data.countAlive; i++)
					{
						for (int j = 0; j < particlesEmiter.updates.size(); j++)
							particlesEmiter.updates[j]->Update(&data, i);
						if (data.time[i] < 0)
							data.kill(i);
					}
				}
			};
		}
	}
}

#endif // !__PARTICLESSYSTEM_HPP__
