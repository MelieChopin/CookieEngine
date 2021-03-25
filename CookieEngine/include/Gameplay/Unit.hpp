#ifndef __UNIT_HPP__
#define __UNIT_HPP__

#include "Living.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		class Unit : Living
		{
		public:
			float moveSpeed = 0;
			bool  isFlying = false;

			float attackDamage = 0;
			float attackSpeed  = 0;
			float attackRange  = 0;


			Unit() {}
			~Unit() {}

			void MoveTo() {}
			void Attack() {}
		};


	}
}

#endif // !__UNIT_HPP__
