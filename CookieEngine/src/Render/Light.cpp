#include "Render/RendererRemote.hpp"
#include "ShadowBuffer.hpp"
#include "Light.hpp"

using namespace Cookie::Render;

/*=============== DIR LIGHTS ===============*/

/*=============== SPHERE LIGHTS ===============*/

/*=============== SPOT LIGHTS ===============*/

/*=============== LIGHTS ARRAY ===============*/

LightsArray::LightsArray()
{
};

void LightsArray::Clear()
{

	for (int i = 0; i < DIR_LIGHT_MAX_NB; i++)
	{
		DirLight& iLight = dirLights.at(i);

		if (iLight.castShadow)
		{
			if (iLight.shadowMap)
			{
				iLight.shadowMap->Clear();
			}
			else
			{
				iLight.shadowMap = std::make_unique<ShadowBuffer>();
			}
		}
		else
		{
			if (iLight.shadowMap)
			{
				iLight.shadowMap.reset();
				iLight.shadowMap = nullptr;
			}
		}
	}
}