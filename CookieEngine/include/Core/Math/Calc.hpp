#ifndef __CALC_HPP__
#define __CALC_HPP__

namespace Cookie
{
	namespace Core
	{
		namespace Math
		{
			#define PI  3.141592f
			#define TAU 6.283185f

			inline float ToRadians(float degrees) { return degrees * (PI / 180.f); }
			inline float ToDegrees(float radians) { return radians * (180.f / PI); }

		}
	}
}
#endif