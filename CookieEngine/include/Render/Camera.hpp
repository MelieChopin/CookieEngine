#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Core/Math/Mat4.hpp"
#include "Core/Math/Calc.hpp"

namespace Cookie
{
	namespace Render
	{
		class Camera
		{
			private:
				Core::Math::Mat4 projMat;
				Core::Math::Mat4 viewMat;

			public:
				Core::Math::Vec3 pos = {0.0f,0.0f,0.0f};
				Core::Math::Vec3 rot = {0.0f,0.0f,0.0f};

			private:

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Camera() {}
				~Camera() {}

				inline Core::Math::Mat4 GetViewProj()const { return projMat * viewMat; }

				inline void SetProj(float yFov, float aspect, float n, float f) { projMat = Core::Math::Mat4::Perspective(yFov, aspect, n, f); }
				
				inline void Update()
				{
					viewMat = Core::Math::Mat4::Translate(-pos) * Core::Math::Mat4::RotateZ(Core::Math::ToRadians(-rot.z)) * Core::Math::Mat4::RotateY(Core::Math::ToRadians(-rot.y)) * Core::Math::Mat4::RotateX(Core::Math::ToRadians(-rot.x));
				}

		};
	}
}

#endif // __CAMERA_HPP__
