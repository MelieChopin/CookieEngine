#ifndef __MAT4_HPP__
#define __MAT4_HPP__

#include "Vec4.hpp"
 
namespace Cookie
{
    namespace Core
    {
        namespace Math
        {

            union Mat4
            {
                float e[16];
                Vec4 c[4];

                inline void Debug() const;
                inline static void DebugAllTest();

                inline static Mat4 Identity();
                inline static Mat4 Scale(float s);
                inline static Mat4 Scale(const Vec3& s);
                inline static Mat4 Translate(const Vec3& t);
                inline static Mat4 RotateX(float radians);
                inline static Mat4 RotateY(float radians);
                inline static Mat4 RotateZ(float radians);
                inline static Mat4 Perspective(float yFov, float aspect, float n, float f);

                inline Mat4 Transpose();
                inline Vec4 operator*(const Vec4& other) const;
                inline Mat4 operator*(const Mat4& other) const;
                inline Mat4& operator*=(const Mat4& other);
            };

        }
    }
}

#include "Mat4.inl"

#endif