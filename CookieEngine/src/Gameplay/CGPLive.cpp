#include "Gameplay/CGPLive.hpp" 
#include <cmath>

using namespace Cookie::Gameplay;


void CGPLive::TakeHit(const float damage)
{
	lifeCurrent -= std::fmax(damage - armor, 0);
}