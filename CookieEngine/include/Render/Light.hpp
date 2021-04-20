#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "Core/Math/Vec3.hpp"
#include <vector>
#include <array>

namespace Cookie
{
	namespace Render
	{
		#define DIR_LIGHT_MAX_NB 10

		class DirLight
		{
			public:
				Core::Math::Vec3 dir;
				Core::Math::Vec3 color = {1.0f,1.0f,1.0f};

		};

		#define SPHERE_LIGHT_MAX_NB 10

		class SphereLight
		{
			public:
				Core::Math::Vec3 pos	= { 0.0f,0.0f,0.0f };
				Core::Math::Vec3 color  = { 1.0f,1.0f,1.0f };

				float radius = 0.0f;
		};

		#define SPOT_LIGHT_MAX_NB 10

		class SpotLight
		{
			public:
				Core::Math::Vec3 pos	= {0.0f,0.0f,0.0f};
				Core::Math::Vec3 dir	= {0.0f,0.0f,0.0f};
				Core::Math::Vec3 color  = {1.0f,1.0f,1.0f};

				float radius	= 0.0f;
				float angle		= 0.0f;
		};


		class LightsArray
		{
			private:
				std::array<DirLight, DIR_LIGHT_MAX_NB>	dirLights;
				std::vector<SphereLight>				sphereLights;
				std::vector<SpotLight>					spotLights;

			public:
				LightsArray();
				LightsArray(const LightsArray& _lights);
		};
	}
}

#endif // !__LIGHT_HPP__
