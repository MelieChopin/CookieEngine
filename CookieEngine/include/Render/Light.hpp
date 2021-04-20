#ifndef __LIGHT_HPP__
#define __LIGHT_HPP__

#include "Core/Math/Vec3.hpp"

namespace Cookie
{
	namespace Render
	{
		class DirLight
		{
			public:
				Core::Math::Vec3 dir;
				Core::Math::Vec3 color = {1.0f,1.0f,1.0f};

		};

		class SphereLight
		{
			public:
				Core::Math::Vec3 pos;
				Core::Math::Vec3 color = { 1.0f,1.0f,1.0f };

				float radius = 0.0f;
		};

		class SpotLight
		{
		public:
			Core::Math::Vec3 pos;
			Core::Math::Vec3 dir;
			Core::Math::Vec3 color = { 1.0f,1.0f,1.0f };

			float radius	= 0.0f;
			float angle		= 0.0f;
		};
	}
}

#endif // !__LIGHT_HPP__
