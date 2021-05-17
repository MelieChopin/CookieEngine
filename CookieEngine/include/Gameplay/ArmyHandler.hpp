#ifndef _ARMY_HANDLER_HPP__
#define _ARMY_HANDLER_HPP__

#include "ECS/ComponentGameplay.hpp"
#include "Gameplay/Income.hpp"

namespace Cookie
{
	namespace Gameplay
	{

		#define MAX_ARMIES 8

		class Army
		{
		public:

			std::string                          name {"No Name"};
			Income                               income;
			std::vector<ECS::ComponentGameplay*> workers;
			std::vector<ECS::ComponentGameplay*> units;
			std::vector<ECS::ComponentGameplay*> buildings;
			
			Army() {}
			~Army() {}

		};

		class ArmyHandler
		{
		public:
			int               livingArmies {0};
			std::array<Army, MAX_ARMIES> armies;

			ArmyHandler() {}
			~ArmyHandler() {}

			void AddElementToArmy(ECS::ComponentGameplay* element);
			void AddElementToArmy(Army& army, ECS::ComponentGameplay* element);

			void RemoveElementFromArmy(ECS::ComponentGameplay* element);
			void RemoveElementFromArmy(Army& army, ECS::ComponentGameplay* element);
			void RemoveElementFromVector(std::vector<ECS::ComponentGameplay*>& vector, ECS::ComponentGameplay* element);

		};

	}
}

#endif // !_ARMY_HANDLER_HPP__