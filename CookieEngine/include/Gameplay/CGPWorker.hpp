#ifndef _CGP_WORKER_HPP__
#define _CGP_WORKER_HPP__

#include "Core/Time.hpp"
#include "ComponentTransform.hpp"
#include "CGPProducer.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		class CGPWorker
		{
		public:

			//will be replace by the CPGMove later on
			ECS::ComponentTransform* trs     {nullptr};
			float moveSpeed                  {5};

			Core::Math::Vec3 posBase         {0, 0, 0};
			Core::Math::Vec3 posResource     {0, 0, 0};
			Income* income                   {nullptr};

			float timeToRecolt               {2};
			float currentCooldown            {0};
			bool isCarryingResource          {false};
			bool isResourcePrimary           {true};

			std::vector<Resources::Prefab*> possibleBuildings;

			CGPWorker() {}
			~CGPWorker() {}

			inline void Update()
			{
				if (currentCooldown > 0)
				{
					currentCooldown -= Core::DeltaTime();
					if (currentCooldown <= 0)
						isCarryingResource = true;

					return;
				}


				//add movement


			}

			inline void ToDefault() noexcept
			{
				posBase                     = {0, 0, 0};
				posResource                 = {0, 0, 0};
				isCarryingResource          = false;
				isResourcePrimary           = false;
			}
		};


	}
}

#endif // !_CGP_WORKER_HPP__