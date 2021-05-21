#ifndef _CGP_PRODUCER_HPP__
#define _CGP_PRODUCER_HPP__

#include "Map.hpp"
#include "Gameplay/Income.hpp"
#include <vector>

namespace Cookie
{
	namespace Resources
	{
		class Prefab;
	}
	namespace ECS
	{
		class Coordinator;
	}

	namespace Gameplay
	{
		
		#define CGP_PRODUCER_MAX_IN_QUEUE 5

		class CGPProducer
		{
		public:
			Income*							income {nullptr};
			std::vector<Resources::Prefab*>	possibleUnits;
			std::vector<Resources::Prefab*> queueOfUnits;
			float                           currentCountdown {0};

			Core::Math::Vec2                tileSize {{0, 0}};
			std::vector<Resources::Tile*>   occupiedTiles;

			CGPProducer() {}
			~CGPProducer() {}


			inline void ToDefault() noexcept
			{
				income = nullptr;
				possibleUnits.clear();
				queueOfUnits.clear();
				currentCountdown = 0;
				
				tileSize = { {0, 0} };
				for (int i = 0; i < occupiedTiles.size(); ++i)
					occupiedTiles[i]->isObstacle = false;
				occupiedTiles.clear();
				
			}
		
			void UpdateCountdown(ECS::Coordinator& coordinator, int selfId);

			bool AddUnitToQueue(int indexInPossible);
			void RemoveUnitFromQueue(int indexInQueue);
		};
	}
}

#endif // !_CGP_PRODUCER_HPP__