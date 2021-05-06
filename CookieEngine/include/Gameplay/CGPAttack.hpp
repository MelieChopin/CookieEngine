#ifndef _CGP_ATTACK_HPP__
#define _CGP_ATTACK_HPP__


namespace Cookie
{
	namespace Gameplay
	{
		class CGPAttack 
		{
		public:
			float attackDamage = 0;
			float attackSpeed = 0;
			float attackRange = 0;

			CGPAttack() {}
			~CGPAttack() {}

			void Attack() {}
			inline void ToDefault() noexcept
			{
				attackDamage = 0;
				attackSpeed = 0;
				attackRange = 0;
			}
		};


	}
}

#endif // !_CGP_ATTACK_HPP__