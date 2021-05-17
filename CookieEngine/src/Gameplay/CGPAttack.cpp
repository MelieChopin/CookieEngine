#include "Gameplay/CGPAttack.hpp"
#include "Core/Time.hpp"

using namespace Cookie::Gameplay;

void CGPAttack::Attack()
{
	//add Check so attackCooldown will not have high negative value if unit stand still long enough
	if (attackCooldown > 0)
		attackCooldown -= Core::DeltaTime();

	if (target && attackCooldown <= 0)
	{
		attackCooldown = 1.f / attackSpeed;
		target->TakeHit(attackDamage);
	}
}