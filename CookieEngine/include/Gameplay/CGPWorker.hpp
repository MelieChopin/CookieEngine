#ifndef _CGP_WORKER_HPP__
#define _CGP_WORKER_HPP__

#include "Core/Time.hpp"
#include "ComponentTransform.hpp"
#include "CGPProducer.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		#define PRIMARY_PER_RECOLT 5
		#define SECONDARY_PER_RECOLT 4

		class CGPWorker
		{
		public:

			//will be replace by the CPGMove later on
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

			inline void ToDefault() noexcept
			{
				posBase                     = {0, 0, 0};
				posResource                 = {0, 0, 0};
				isCarryingResource          = false;
				isResourcePrimary           = false;
			}

			inline void Update(ECS::ComponentTransform& trs)
			{/*
				if (currentCooldown > 0)
				{
					currentCooldown -= Core::DeltaTime();
					if (currentCooldown <= 0)
						isCarryingResource = true;

					return;
				}

				Core::Math::Vec3 destination = (isCarryingResource) ? posBase : posResource;
				Core::Math::Vec3 direction = (destination - trs.pos).Normalize();
				trs.pos += direction * (moveSpeed * Core::DeltaTime());
				trs.trsHasChanged = true;

				//if reach Destination
				if ((destination - trs.pos).Length() < 0.1)
				{
					if (isCarryingResource)
					{
						isCarryingResource = false;
						(isResourcePrimary ? income->primary : income->secondary) += (isResourcePrimary ? PRIMARY_PER_RECOLT : SECONDARY_PER_RECOLT);
					}
					else
						currentCooldown = timeToRecolt;
				}

				*/
			}


		};

	}
}

#endif // !_CGP_WORKER_HPP__