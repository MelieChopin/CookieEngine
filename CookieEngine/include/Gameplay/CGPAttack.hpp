#ifndef _CGP_ATTACK_HPP__
#define _CGP_ATTACK_HPP__

#include "CGPLive.hpp"

namespace Cookie
{
	namespace Gameplay
	{
		class CGPAttack 
		{
		public:
			bool needToAttack    {false};

			float powerLevel     {0}; //used for AI analysis
			float attackDamage   {1};
			float attackSpeed    {5}; //attack per sec, so maxCooldown = 1 / attackSpeed
			float attackCooldown {0};
			float attackRange    {10};

			CGPLive* target {nullptr}; 


			CGPAttack() {}
			~CGPAttack() {}

			inline void ToDefault() noexcept
			{
				needToAttack = false;
				attackDamage = 0;
				attackSpeed = 0;
				attackRange = 0;
				target = nullptr;
			}

			void Attack();

		};


	}
}

#endif // !_CGP_ATTACK_HPP__