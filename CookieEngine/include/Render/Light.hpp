#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "Core/Math/Vec3.hpp"
#include <vector>
#include <array>

namespace Cookie
{
	namespace Render
	{
		constexpr const char* blinnPhong = R"(

		cbuffer Cam : register(b1)
		{
		    float3 camPos;
		};
		
		float	compute_diffuse		(float3 lightDir, float3 normal)
		{
			float diff 			= max(dot(normal,lightDir),0.0);
		
			return diff;
		}
		
		float	compute_specular	(float3 lightDir, float3 normal, float3 position, float shininess)
		{
			float3 viewDir		= normalize(camPos - position);
			float3 halfAngleVec	= normalize(lightDir + viewDir);
		
			float spec			= pow(max(dot(normal,halfAngleVec),0.0f),shininess);
		
			return  spec;
		}

		float compute_lighting(float3 normal, float3 position, float3 lightDir, float metalic, float roughness)
		{
			float shininess = (2/(roughness*roughness)) - 2;
			float specular	= compute_specular(lightDir,normal,position,shininess);
			float diffuse	= compute_diffuse(lightDir,normal);

			return (0.1 + diffuse + specular);
		}

		)";

		#define DIR_LIGHT_MAX_NB 10

		struct DirLight
		{
				Core::Math::Vec3 dir;
				Core::Math::Vec3 color = {1.0f,1.0f,1.0f};
		};

		#define SPHERE_LIGHT_MAX_NB 10

		struct SphereLight
		{
				Core::Math::Vec3 pos	= { 0.0f,0.0f,0.0f };
				Core::Math::Vec3 color  = { 1.0f,1.0f,1.0f };

				float radius = 0.0f;
		};

		#define SPOT_LIGHT_MAX_NB 10

		struct SpotLight
		{
				Core::Math::Vec3 pos	= {0.0f,0.0f,0.0f};
				Core::Math::Vec3 dir	= {0.0f,0.0f,0.0f};
				Core::Math::Vec3 color  = {1.0f,1.0f,1.0f};

				float radius	= 0.0f;
				float angle		= 0.0f;
		};


		struct LightsArray
		{
			public:
				std::array<DirLight, DIR_LIGHT_MAX_NB>	dirLights;
				unsigned int							usedDir = 0;
				//std::vector<SphereLight>				sphereLights;
				//std::vector<SpotLight>					spotLights;


			public:
				LightsArray();
				LightsArray(const LightsArray& _lights);
		};
	}
}

#endif // !__LIGHT_HPP__
