#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "Core/Math/Mat4.hpp"

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
				Core::Math::Mat4 projMat = Core::Math::Mat4::Identity();

				float fov = 0.0f;

			protected:
				Core::Math::Mat4 viewMat = Core::Math::Mat4::Identity();

				bool activated = true;

				float previousMouseX{ 0.0 };
				float previousMouseY{ 0.0 };

			public:
				float camFar = 0.0f;

				float width = 0.0f;
				float height = 0.0f;

				Core::Math::Vec2 windowOffset = { {0.0f,0.0f} };

				Core::Math::Vec3 pos = {0.0f,0.0f,0.0f};
				Core::Math::Vec3 rot = {0.0f,0.0f,0.0f};
				
			public:
				Camera() {}
				~Camera() {}

				inline const Core::Math::Mat4& GetView() const	{ return viewMat;			}
				inline const Core::Math::Mat4& GetProj() const	{ return projMat;			}
				inline Core::Math::Mat4 GetViewProj() const		{ return projMat * viewMat;	}
				
				inline void SetProj(float yFov, float _width, float _height, float n, float f) { fov = yFov; width = _width; height = _height; camFar = f; projMat = Core::Math::Mat4::Perspective(yFov, width / height, n, f); }
					    
				inline virtual void Update() = 0;

				inline void Activate() { activated = true; }
				inline void Deactivate() { activated = false; }

				inline virtual void ResetPreviousMousePos();

				inline Core::Math::Vec3 MouseToWorldDir();
					   
		};

		class FreeFlyCam : public Camera
		{
			private:


			public:
				FreeFlyCam() {}
				virtual ~FreeFlyCam() {}

			inline void UpdateFreeFlyPos();
			inline void UpdateFreeFlyRot();
			inline void Update() override;

		};

		class GameCam : public Camera
		{
		private:


		public:
			GameCam() {}
			virtual ~GameCam() {}

			inline void UpdateGamePos();
			inline void UpdateZoom();
			inline void Update() override;
		};
	}
}

#include "Camera.inl"

#endif // __CAMERA_HPP__
