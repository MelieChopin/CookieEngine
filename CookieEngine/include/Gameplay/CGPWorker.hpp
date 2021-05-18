#ifndef _CGP_WORKER_HPP__
#define _CGP_WORKER_HPP__

#include "ComponentTransform.hpp"
#include "Gameplay/Income.hpp"
#include <vector>

namespace Cookie
{
	namespace ECS
	{
		class Coordinator;
	}

	namespace Resources
	{
		class Prefab;
	}

	namespace Gameplay
	{
		#define TIME_TO_HARVEST 2
		#define PRIMARY_PER_RECOLT 5
		#define SECONDARY_PER_RECOLT 4

		class CGPWorker
		{
		public:

			//will be replace by the CPGMove with flying later on
			float moveSpeed                  {5};

			Core::Math::Vec3 posBase         {0,  1, 0};
			Core::Math::Vec3 posResource     {25, 1, 0};
			Income* income                   {nullptr};

			float harvestCountdown           {0};
			bool isCarryingResource          {false};
			bool isResourcePrimary           {true};

			std::vector<Resources::Prefab*>	possibleBuildings;
			Resources::Prefab*				BuildingInConstruction {nullptr};
			float                           constructionCountdown  {0};

			CGPWorker() {}
			~CGPWorker() {}

			inline void ToDefault() noexcept
			{
				income						= nullptr;
				posBase                     = {0, 0, 0};
				posResource                 = {0, 0, 0};
				harvestCountdown            = 0;
				isCarryingResource          = false;
				isResourcePrimary           = false;
				BuildingInConstruction       = nullptr;
				constructionCountdown       = 0;
			}

			void Update(ECS::ComponentTransform& trs, ECS::Coordinator& coordinator);


		};

	}
}

#endif // !_CGP_WORKER_HPP__