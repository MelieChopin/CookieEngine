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


void LightsArray::Resize(int width, int height)
{
	for (int i = 0; i < DIR_LIGHT_MAX_NB; i++)
	{
		DirLight& iLight = dirLights.at(i);

		if (iLight.castShadow)
		{
			if (iLight.shadowMap)
			{
				iLight.shadowMap->Resize(width, height);
			}
			else
			{
				iLight.shadowMap = std::make_unique<ShadowBuffer>(width, height);
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

void LightsArray::Clear()
{
	for (int i = 0; i < DIR_LIGHT_MAX_NB; i++)
	{
		DirLight& iLight = dirLights.at(i);

		if (iLight.castShadow)
		{
			if (iLight.shadowMap)
			{
				Render::RendererRemote::context->ClearDepthStencilView(iLight.shadowMap->depthStencilView, D3D11_CLEAR_DEPTH , 1.0f, 0.0f); 
			}
		}
	}
}