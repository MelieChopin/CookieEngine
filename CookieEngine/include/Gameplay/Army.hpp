#ifndef _ARMY_HPP__
#define _ARMY_HPP__

#include "ECS/ComponentGameplay.hpp"
#include "CGPProducer.hpp"

namespace Cookie
{
	namespace Gameplay
	{

		class Army
		{
			std::string                          name {"No Name"};
			Income                               income;
			std::vector<ECS::ComponentGameplay*> workers;
			std::vector<ECS::ComponentGameplay*> units;
			std::vector<ECS::ComponentGameplay*> buildings;
			
			Army() {}
			~Army() {}

		};


	}
}

#endif // !_ARMY_HPP__