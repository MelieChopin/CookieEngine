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
			float attackDamage   {1};
			float attackSpeed    {5}; //attack per sec, so maxCooldown = 1 / attackSpeed
			float attackCooldown {0};
			float attackRange    {10};

			CGPLive* target {nullptr}; 


			CGPAttack() {}
			~CGPAttack() {}

			void Attack() 
			{
				//add Check so attackCooldown will not have high negative value if unit stand still long enough
				if(attackCooldown > 0)
					attackCooldown -= Core::DeltaTime();

				if (target && attackCooldown <= 0)
				{
					attackCooldown = 1.f / attackSpeed;
					target->TakeHit(attackDamage);
				}
			}

			inline void ToDefault() noexcept
			{
				needToAttack = false;
				attackDamage = 0;
				attackSpeed = 0;
				attackRange = 0;
				target = nullptr;
			}
		};


	}
}

#endif // !_CGP_ATTACK_HPP__