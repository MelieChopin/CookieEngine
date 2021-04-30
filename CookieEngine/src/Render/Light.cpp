#include "Light.hpp"

using namespace Cookie::Render;

/*=============== DIR LIGHTS ===============*/

/*=============== SPHERE LIGHTS ===============*/

/*=============== SPOT LIGHTS ===============*/

/*=============== LIGHTS ARRAY ===============*/

LightsArray::LightsArray()
{
};

LightsArray::LightsArray(const LightsArray& _lights):
	dirLights		{_lights.dirLights}
{
};