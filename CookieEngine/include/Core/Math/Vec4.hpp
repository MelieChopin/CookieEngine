#ifndef __VEC4_HPP__
#define __VEC4_HPP__

#include "Vec3.hpp"

namespace Cookie
{
    namespace Core
    {
        namespace Math
        {

            union Vec4
            {
                Vec4() = default;
                Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
                Vec4(Vec3 _xyz, float _w) : x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w) {}

                struct { float x; float y; float z; float w; };
                struct { float r; float g; float b; float a; };
                float e[4];

                inline void Debug() const { { std::cout << "{" << x << " ," << y << " ," << z << " ," << w << "}\n"; } }
            };

        }
    }
}

#endif