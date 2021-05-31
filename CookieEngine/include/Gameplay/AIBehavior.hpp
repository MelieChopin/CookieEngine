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
			std::string			name {"No Name"}; //for UI and Save
			std::vector<AIStep> steps;

			int					currentStepIndex {0};
			AIStep				stepGoals; //total

			void AddNextStep()
			{

				if (currentStepIndex < steps.size())
				{
					stepGoals += steps[currentStepIndex];
					currentStepIndex++;
				}
				//else loop with last Step goals
			}
			void Clear()
			{
				name = "No Name";
				steps.clear();
				currentStepIndex = 0;
				stepGoals.Clear();
			}
		};
	}
}

#endif // !_AIBEHAVIOR_HPP__