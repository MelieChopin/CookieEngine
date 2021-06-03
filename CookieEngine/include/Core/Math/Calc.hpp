#ifndef __CALC_HPP__
#define __CALC_HPP__

namespace Cookie
{
	namespace Core
	{
		namespace Math
		{
			constexpr float PI = 3.141592f;
			constexpr float TAU = 6.283185f;

			inline float ToRadians(float degrees) { return degrees * (PI / 180.f); }
			inline float ToDegrees(float radians) { return radians * (180.f / PI); }
			inline float Lerp(float a, float b, float f) { return a + f * (b - a); }
			inline Cookie::Core::Math::Vec4 Random(Cookie::Core::Math::Vec4& min, Cookie::Core::Math::Vec4& max)
			{
				return Cookie::Core::Math::Vec4(float(rand()) / float((RAND_MAX)) * (max.x - min.x) + min.x,
					float(rand()) / float((RAND_MAX)) * (max.y - min.y) + min.y,
					float(rand()) / float((RAND_MAX)) * (max.z - min.z) + min.z,
					float(rand()) / float((RAND_MAX)) * (max.w - min.w) + min.w);
			}
			inline Cookie::Core::Math::Vec3 Random(Cookie::Core::Math::Vec3& min, Cookie::Core::Math::Vec3& max)
			{
				return Cookie::Core::Math::Vec3(float(rand()) / float((RAND_MAX)) * (max.x - min.x) + min.x,
					float(rand()) / float((RAND_MAX)) * (max.y - min.y) + min.y,
					float(rand()) / float((RAND_MAX)) * (max.z - min.z) + min.z);
			}
		}
	}
}
#endif