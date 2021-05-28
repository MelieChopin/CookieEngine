#ifndef _AIBEHAVIOR_HPP__
#define _AIBEHAVIOR_HPP__

#include <algorithm>
#include "Gameplay/AIStep.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		struct AIBehavior
		{
			std::vector<AIStep> steps;
			int					currentStepIndex {-1};

			AIStep				stepGoals;

			void AddNextStep()
			{
				currentStepIndex++;

				if(currentStepIndex < steps.size())
					stepGoals += steps[currentStepIndex];
				//else loop with last Step goals
			}

		};
	}
}

#endif // !_AIBEHAVIOR_HPP__