#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Core/Math/Mat4.hpp"
#include "Core/Math/Calc.hpp"


#include "Core/Time.hpp"

namespace Cookie
{
	namespace Render
	{
		#define CAM_MOUSE_SPEED 50.f
		#define CAM_MOUSE_SPEED_UP_SCALE 10.f
		#define CAM_MOUSE_SENSITIVITY_X 0.0007f
		#define CAM_MOUSE_SENSITIVITY_Y 0.0007f

		#define CAMERA_INITIAL_NEAR 0.01f
		#define CAMERA_INITIAL_FAR  10000.f

		class Camera
		{
			private:
				Core::Math::Mat4 projMat;
				Core::Math::Mat4 viewMat;

			protected:
				bool activated = true;

				float previousMouseX{ 0.0 };
				float previousMouseY{ 0.0 };

			public:
				Core::Math::Vec3 pos = {0.0f,0.0f,0.0f};
				Core::Math::Vec3 rot = {0.0f,0.0f,0.0f};

			private:

			public:
				Camera() {}
				~Camera() {}

				inline Core::Math::Mat4 GetViewProj() const {return projMat * viewMat;}
				inline void SetProj(float yFov, float aspect, float n, float f) {projMat = Core::Math::Mat4::Perspective(yFov, aspect, n, f);}
					    
				inline virtual void Update() = 0;

				inline void Activate() { activated = true; }
				inline void Deactivate() { activated = false; }

				inline virtual void ResetPreviousMousePos();
					   
		};

		class FreeFlyCam : public Camera
		{
			private:


			public:
				FreeFlyCam() {}
				~FreeFlyCam() {}

			inline void UpdateFreeFlyPos();
			inline void UpdateFreeFlyRot();
			inline void Update()override;

		};
	}
}

#include "Camera.inl"

#endif // __CAMERA_HPP__
