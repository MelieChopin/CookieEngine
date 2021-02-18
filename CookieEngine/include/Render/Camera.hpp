#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Core/Math/Mat4.hpp"

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
				Core::Math::Vec3 pos;
				Core::Math::Vec3 rot;

			private:

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Camera() {}
				~Camera() {}

				inline Core::Math::Mat4 GetViewProj() { return projMat * viewMat; }
				inline void SetProj(float yFov, float aspect, float n, float f) { projMat = Core::Math::Mat4::Perspective(yFov, aspect, n, f); }
				
				inline void Update()
				{
					viewMat = Core::Math::Mat4::Translate(-pos) * Core::Math::Mat4::RotateZ(-rot.z) * Core::Math::Mat4::RotateY(-rot.y) * Core::Math::Mat4::RotateX(-rot.x);
				}

		};
	}
}

#endif // __CAMERA_HPP__
