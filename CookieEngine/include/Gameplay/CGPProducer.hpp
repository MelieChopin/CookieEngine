#ifndef _CGP_PRODUCER_HPP__
#define _CGP_PRODUCER_HPP__

#include <vector>
#include "Core/Time.hpp"
#include "Resources/Prefab.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		
		#define CGP_PRODUCER_MAX_IN_QUEUE 5

		struct Income
		{
			float primary    {0};
			float secondary  {0};
		};


		struct InstantiableUnit
		{
			Resources::Prefab* unit {nullptr};
			float timeToProduce     {0};
			float costPrimary       {0};
			float costSecondary     {0};
		};


		class CGPProducer
		{
		public:
			std::vector<InstantiableUnit*> possibleUnits;
			std::vector<InstantiableUnit*> queueOfUnits;
			float                          currentCooldown {0};

			CGPProducer() {}
			~CGPProducer() {}


			inline void ToDefault() noexcept
			{
				possibleUnits.clear();
				queueOfUnits.clear();
				currentCooldown = 0;
			}
		
			void UpdateCooldown()
			{
				if (queueOfUnits.empty)
					return;

				currentCooldown -= Core::DeltaTime;

				if (currentCooldownc <= 0)
				{
					//instantiate queueOfUnits[0]->unit
					queueOfUnits.erase(queueOfUnits.begin);

					if (!queueOfUnits.empty)
						currentCooldown = queueOfUnits[0]->timeToProduce;
				}

			}
			bool AddUnitToQueue(InstantiableUnit* unitToAdd, Income& income)
			{
				//later on add Debug.Log depending on what is blocking the process to give player Feedback
				if (queueOfUnits.size() == CGP_PRODUCER_MAX_IN_QUEUE ||
					unitToAdd->costPrimary > income.primary ||
					unitToAdd->costSecondary > income.secondary)
					return false;

				income.primary   -= unitToAdd->costPrimary
				income.secondary -= unitToAdd->costSecondary;
				queueOfUnits.push_back(unitToAdd);
				return true;
			}
			void RemoveUnitFromQueue(int indexInQueue, Income& income)
			{
				//should be impossible when UI implemented 
				if (indexInQueue > queueOfUnits.size())
					return;

				income.primary   += queueOfUnits[indexInQueue]->costPrimary;
				income.secondary += queueOfUnits[indexInQueue]->costSecondary;
				queueOfUnits.erase(queueOfUnits.begin + indexInQueue);

				if (indexInQueue == 0 && !queueOfUnits.empty)
					currentCooldown = queueOfUnits[0]->timeToProduce;
			}
			};
	}
}

#endif // !_CGP_PRODUCER_HPP__