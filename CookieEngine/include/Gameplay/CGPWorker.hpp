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
		class Map;
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

			//Harvest
			Income* income                   {nullptr};
			Core::Math::Vec3* posBase        {nullptr};
			Core::Math::Vec3  posResource    {25, 0, 0};
			float harvestCountdown           {0};
			bool isCarryingResource          {false};
			bool isResourcePrimary           {true};

			//Building
			std::vector<Resources::Prefab*>	possibleBuildings;
			Resources::Prefab*				BuildingInConstruction {nullptr};
			Core::Math::Vec3				posBuilding            {0,  0, 0}; // = mousePos when start construction
			bool							needTostartBuilding    {false};
			float                           constructionCountdown  {0};

			CGPWorker() {}
			~CGPWorker() {}

			inline void ToDefault() noexcept
			{
				income						= nullptr;
				posBase                     = nullptr;
				posResource                 = {0, 0, 0};
				harvestCountdown            = 0;
				isCarryingResource          = false;
				isResourcePrimary           = false;
				BuildingInConstruction       = nullptr;
				constructionCountdown       = 0;
			}

			void Update(Resources::Map& map, ECS::Coordinator& coordinator, int selfId);

			void StartBuilding(Core::Math::Vec3& _posBuilding, int indexInPossibleBuildings);
		};

	}
}

#endif // !_CGP_WORKER_HPP__
