#ifndef _AISTEP_HPP__
#define _AISTEP_HPP__

#include <vector>
#include <string>

namespace Cookie
{
	namespace Gameplay
	{
		struct AIStep
		{
			int nbOfWorker {0};
			int nbOfUnits  {0};  //maybe use powerLevel instead
			std::vector<std::string> listOfBuildings;

			AIStep operator+(const AIStep& other)
			{
				AIStep toreturn = *this;

				toreturn.nbOfWorker += other.nbOfWorker;
				toreturn.nbOfUnits  += other.nbOfUnits;

				for (int i = 0; i < other.listOfBuildings.size(); ++i)
					toreturn.listOfBuildings.push_back(other.listOfBuildings[i]);

				return toreturn;
			}

			AIStep& operator+=(const AIStep& other) { *this = *this + other; return *this; }
		};
	}
}

#endif // !_AISTEP_HPP__