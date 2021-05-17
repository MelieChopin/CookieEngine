#include "Gameplay/CGPLive.hpp" 
#include <cmath>

using namespace Cookie::Gameplay;


void CGPLive::TakeHit(const float damage)
{
	life -= std::fmax(damage - armor, 0);
}