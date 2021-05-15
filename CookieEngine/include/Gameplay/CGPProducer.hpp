#ifndef _CGP_PRODUCER_HPP__
#define _CGP_PRODUCER_HPP__

#include <vector>
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"
#include "Resources/Map.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		
		#define CGP_PRODUCER_MAX_IN_QUEUE 5

		struct Income
		{
			float primary       {0};
			float secondary     {0};
			float supplyCurrent {0};
			float supplyMax     {0};
		};


		class CGPProducer
		{
		public:
			std::vector<Resources::Prefab*> possibleUnits;
			std::vector<Resources::Prefab*> queueOfUnits;
			float                          currentCooldown {0};

			Core::Math::Vec2               tileSize{ {0, 0} };
			std::vector<Resources::Tile*>  occupiedTiles;

			CGPProducer() {}
			~CGPProducer() {}


			inline void ToDefault() noexcept
			{
				possibleUnits.clear();
				queueOfUnits.clear();
				currentCooldown = 0;
				
				tileSize = { {0, 0} };
				for (int i = 0; i < occupiedTiles.size(); ++i)
					occupiedTiles[i]->isObstacle = false;
				occupiedTiles.clear();
				
			}
		
			void UpdateCooldown()
			{/*
				if (queueOfUnits.empty())
					return;

				currentCooldown -= Core::DeltaTime();

				if (currentCooldown <= 0)
				{
					//instantiate queueOfUnits[0]->unit
					queueOfUnits.erase(queueOfUnits.begin());

					if (!queueOfUnits.empty())
						currentCooldown = queueOfUnits[0]->timeToProduce;
				}
				*/
			}
			bool AddUnitToQueue(Resources::Prefab* unitToAdd, Income& income)
			{/*
				//later on add Debug.Log depending on what is blocking the process to give player Feedback
				if (queueOfUnits.size() == CGP_PRODUCER_MAX_IN_QUEUE ||
					unitToAdd->costPrimary > income.primary ||
					unitToAdd->costSecondary > income.secondary ||
					unitToAdd->costSupply + income.supplyCurrent > income.supplyMax)
					return false;

				income.primary -= unitToAdd->costPrimary;
				income.secondary -= unitToAdd->costSecondary;
				income.supplyCurrent += unitToAdd->costSupply;
				queueOfUnits.push_back(unitToAdd);*/
				return true;
			}
			void RemoveUnitFromQueue(int indexInQueue, Income& income)
			{/*
				//should be impossible when UI implemented 
				if (indexInQueue > queueOfUnits.size())
					return;

				income.primary   += queueOfUnits[indexInQueue]->costPrimary;
				income.secondary += queueOfUnits[indexInQueue]->costSecondary;
				income.supplyCurrent -= queueOfUnits[indexInQueue]->costSupply;
				queueOfUnits.erase(queueOfUnits.begin() + indexInQueue);

				if (indexInQueue == 0 && !queueOfUnits.empty())
					currentCooldown = queueOfUnits[0]->timeToProduce;*/
			}
		};
	}
}

#endif // !_CGP_PRODUCER_HPP__