#ifndef __MAT4_INL__
#define __MAT4_INL__


namespace Cookie
{
    namespace Core
    {
        namespace Math
        {

            inline void Mat4::Debug() const
            {
                for (int i = 0; i < 4; ++i)
                    c[i].Debug();
            }

            inline void Mat4::DebugAllTest()
            {
                std::cout << "/////////////////////////////////////////////////\n";
                std::cout << "Mat4 : DebugAllTest\n";

                Mat4 temp = Mat4::Identity();

                //Transformations
                {
                    std::cout << "test Identity :\n";
                    temp.Debug();

                    temp = Mat4::Scale(3);
                    std::cout << "test Scale Uniform with 3 :\n";
                    temp.Debug();

                    temp = Mat4::Scale({ 1, 2, 3 });
                    std::cout << "test Scale with {1, 2, 3} :\n";
                    temp.Debug();

                    temp = Mat4::Translate({ 1, 2, 3 });
                    std::cout << "test Translate with {1, 2, 3} :\n";
                    temp.Debug();

                    temp = Mat4::RotateX(90);
                    std::cout << "test RotateX with 90 :\n";
                    temp.Debug();

                    temp = Mat4::RotateY(90);
                    std::cout << "test RotateY with 90 :\n";
                    temp.Debug();

                    temp = Mat4::RotateZ(90);
                    std::cout << "test RotateZ with 90 :\n";
                    temp.Debug();
                                        
                    temp = Mat4::TRS(Vec3(10, 20, 30), Vec3(ToRadians(90), ToRadians(180), ToRadians(270)), Vec3(1, 2, 3));
                    std::cout << "test TRS with T(10, 20, 30), R(rad(90), rad(180), rad(270)), S(1, 2, 3) :\n";
                    temp.Debug();

                    temp = Mat4::Perspective(45, 800 / 600, 0.01f, 50.f);
                    std::cout << "test Perspective with Fov = 45, Ratio = 800/600, Near = 0.01, Far = 50 :\n";
                    temp.Debug();
                }

                //Operations
                {
                    temp = Mat4::Identity().Transpose();
                    std::cout << "test Identity Transposed :\n";
                    temp.Debug();


                    Vec4 tempVec4 = Mat4::Translate({ 1, 2, 3 }) * Vec4 { 1, 2, 3, 4 };
                    std::cout << "test Mat4 * Vec4, Mat4 is a translate with {1, 2, 3}, Vec4 is {1, 2, 3, 4} :\n";
                    tempVec4.Debug();

                    temp = Mat4::Translate({ 1, 2, 3 }) * Mat4::Translate({ 1, 2, 3 });
                    std::cout << "test Mat4 * Mat4, both are a translate with {1, 2, 3} :\n";
                    temp.Debug();

                    temp = Mat4::Translate({ 1, 2, 3 });
                    temp *= Mat4::Translate({ 1, 2, 3 });
                    std::cout << "test Mat4 *= Mat4, both are a translate with {1, 2, 3} :\n";
                    temp.Debug();
                }

                std::cout << "/////////////////////////////////////////////////\n";
            }

            inline Mat4 Mat4::Identity()
            {
                return
                { {
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 0.f, 1.f
                } };
            }
            inline Mat4 Mat4::Scale(float s)
            {
                return
                { {
                     s,  0.f, 0.f, 0.f,
                    0.f,   s, 0.f, 0.f,
                    0.f, 0.f,   s, 0.f,
                    0.f, 0.f, 0.f, 1.f
                } };
            }
            inline Mat4 Mat4::Scale(const Vec3& s)
            {
                return
                { {
                    s.x, 0.f, 0.f, 0.f,
                    0.f, s.y, 0.f, 0.f,
                    0.f, 0.f, s.z, 0.f,
                    0.f, 0.f, 0.f, 1.f
                } };
            }
            inline Mat4 Mat4::Translate(const Vec3& t)
            {
                return
                { {
                    1.f, 0.f, 0.f, t.x,
                    0.f, 1.f, 0.f, t.y,
                    0.f, 0.f, 1.f, t.z,
                    0.f, 0.f, 0.f, 1.f
                } };
            }
            inline Mat4 Mat4::RotateX(float radians)
            {
                float c = std::cos(radians);
                float s = std::sin(radians);
                return
                { {
                    1.f, 0.f, 0.f, 0.f,
                    0.f,   c,   s, 0.f,
                    0.f,  -s,   c, 0.f,
                    0.f, 0.f, 0.f, 1.f,
                } };
            }
            inline Mat4 Mat4::RotateY(float radians)
            {
                float c = std::cos(radians);
                float s = std::sin(radians);
                return
                { {
                     c, 0.f,  -s, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                     s, 0.f,   c, 0.f,
                    0.f, 0.f, 0.f, 1.f,
                } };
            }
            inline Mat4 Mat4::RotateZ(float radians)
            {
                float c = std::cos(radians);
                float s = std::sin(radians);
                return
                { {
                        c,   s,  0.f, 0.f,
                        -s,   c,  0.f, 0.f,
                        0.f, 0.f, 1.f, 0.f,
                        0.f, 0.f, 0.f, 1.f,
                } };
            }
            inline Mat4 Mat4::TRS(const Vec3& t, const Vec3& r, const Vec3& s)
            {
                return Mat4::Translate(t) * Mat4::RotateY(r.y) *  Mat4::RotateX(r.x)*  Mat4::RotateZ(r.z)* Mat4::Scale(s);
            }
            inline Mat4 Mat4::Perspective(float yFov, float aspect, float n, float f)
            {
                Mat4 m;
                float const a = 1.f / std::tan(yFov / 2.f);

                m.c[0].e[0] = a / aspect;
                m.c[0].e[1] = 0.f;
                m.c[0].e[2] = 0.f;
                m.c[0].e[3] = 0.f;

                m.c[1].e[0] = 0.f;
                m.c[1].e[1] = a;
                m.c[1].e[2] = 0.f;
                m.c[1].e[3] = 0.f;

                m.c[2].e[0] = 0.f;
                m.c[2].e[1] = 0.f;
                m.c[2].e[2] = -((f + n) / (f - n));
                m.c[2].e[3] = -((2.f * f * n) / (f - n)) ;

                m.c[3].e[0] = 0.f;
                m.c[3].e[1] = 0.f;
                m.c[3].e[2] = -1.f;
                m.c[3].e[3] = 0.f;

                return m;
            }

            inline Mat4 Mat4::Transpose()
            {
                return
                { {
                    c[0].e[0], c[1].e[0], c[2].e[0], c[3].e[0],
                    c[0].e[1], c[1].e[1], c[2].e[1], c[3].e[1],
                    c[0].e[2], c[1].e[2], c[2].e[2], c[3].e[2],
                    c[0].e[3], c[1].e[3], c[2].e[3], c[3].e[3],
                } };
            }

            inline Mat4 Mat4::operator*(const Mat4& other) const
            {
                Mat4 res{};
                for (int c = 0; c < 4; ++c)
                    for (int r = 0; r < 4; ++r)
                        for (int k = 0; k < 4; ++k)
                            res.c[c].e[r] += this->c[c].e[k] * other.c[k].e[r];
                return res;
            }
            inline Vec4 Mat4::operator*(const Vec4& other) const
            {
                Vec4 r;
                r.x = other.x * c[0].e[0] + other.y * c[1].e[0] + other.z * c[2].e[0] + other.w * c[3].e[0];
                r.y = other.x * c[0].e[1] + other.y * c[1].e[1] + other.z * c[2].e[1] + other.w * c[3].e[1];
                r.z = other.x * c[0].e[2] + other.y * c[1].e[2] + other.z * c[2].e[2] + other.w * c[3].e[2];
                r.w = other.x * c[0].e[3] + other.y * c[1].e[3] + other.z * c[2].e[3] + other.w * c[3].e[3];
                return r;
            }
            inline Mat4& Mat4::operator*=(const Mat4& other) { *this = *this * other; return *this; }

        }
    }
}

#endif