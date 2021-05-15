#ifndef _ARMY_HANDLER_HPP__
#define _ARMY_HANDLER_HPP__

#include "ECS/ComponentGameplay.hpp"
#include "CGPProducer.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		enum E_ARMY_TYPE
		{
			E_WORKER,
			E_UNIT,
			E_BUILDING
		};


		class Army
		{
			std::string                          name {"No Name"};
			Income                               income;
			std::vector<ECS::ComponentGameplay*> workers;
			std::vector<ECS::ComponentGameplay*> units;
			std::vector<ECS::ComponentGameplay*> buildings;
			
			Army(std::string _name) : name{_name} {}
			~Army() {}

		};

		class ArmyHandler
		{
			std::vector<Army> armies;

			ArmyHandler() {}
			~ArmyHandler() {}

			void AddElementToArmy(ECS::ComponentGameplay* element, E_ARMY_TYPE elementType)
			{
				for (int i = 0; i < armies.size(); ++i)
					if (armies[i].name == element->teamName)
					{
						AddElementToArmy(armies[i], element, elementType)
						return;
					}

				armies.push_back( Army{element->teamName} );
				AddElementToArmy(armies[armies.size() - 1], element, elementType);
			}
			void AddElementToArmy(Army& army, ECS::ComponentGameplay* element, E_ARMY_TYPE elementType)
			{
				switch (elementType)
				{
				case E_ARMY_TYPE::E_WORKER:
					army.workers.push_back(element);
					break;

				case E_ARMY_TYPE::E_UNIT:
					army.units.push_back(element);
					break;

				case E_ARMY_TYPE::E_BUILDING:
					army.buildings.push_back(element);
					break;

				default:
					break;
				}
			}

			void RemoveElementFromArmy(ECS::ComponentGameplay* element, E_ARMY_TYPE elementType)
			{
				for (int i = 0; i < armies.size(); ++i)
					if (armies[i].name == element->teamName)
					{
						RemoveElementFromArmy(armies[i], element, elementType)
						return;
					}
			}
			void RemoveElementFromArmy(Army& army, ECS::ComponentGameplay* element, E_ARMY_TYPE elementType)
			{
				switch (elementType)
				{
				case E_ARMY_TYPE::E_WORKER:
					RemoveElementFromVector(army.workers , element)
					break;

				case E_ARMY_TYPE::E_UNIT:
					RemoveElementFromVector(army.units, element)
					break;

				case E_ARMY_TYPE::E_BUILDING:
					RemoveElementFromVector(army.building, element)
					break;

				default:
					break;
				}
			}
			void RemoveElementFromVector(std::vector<ECS::ComponentGameplay*>& vector, ECS::ComponentGameplay* element)
			{
				for(int i = 0; i < vector.size(); ++i)
					if (vector[i] == element)
					{
						vector.erase(vector.begin + i);
						return;
					}
			}

		};

	}
}

#endif // !_ARMY_HANDLER_HPP__