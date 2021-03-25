#ifndef __VIEWPORT_W_HPP__
#define __VIEWPORT_W_HPP__

#include "UIwidgetBases.hpp"

struct GLFWwindow;
namespace Cookie::Core::Math	{ union Vec2; }
namespace Cookie::Render		{ class FrameBuffer; class Camera; }
namespace Cookie::ECS			{ class Coordinator; class Entity; }


namespace Cookie::UIwidget
{
	class Toolbar;


	class Viewport final : public WindowBase
	{
		Toolbar* toolbar;

		
		struct { float posx, posy, width, height; } viewportDrawspace;


		GLFWwindow* window;

		const Cookie::Render::FrameBuffer& frameBuffer;
		Cookie::Render::Camera* camera;


		Cookie::ECS::Coordinator&	coordinator;
		Cookie::ECS::Entity*&		selectedEntity;


	private:
		void RenderView();
		void GizmoManipulator();

	public:
		inline Viewport(Toolbar*									_toolbar, 
						GLFWwindow*									_window, 
						const Cookie::Render::FrameBuffer&			_frameBuffer, 
						Cookie::Render::Camera*						_camera, 
						Cookie::ECS::Coordinator&					_coordinator,
						Cookie::ECS::Entity*&						_selectedEntity)

			: WindowBase		("Viewport"),
			  toolbar			(_toolbar),
			  window			(_window),
			  frameBuffer		(_frameBuffer),
			  camera			(_camera),
			  coordinator		(_coordinator),
			  selectedEntity	(_selectedEntity)
		{}

		void WindowDisplay() override;
	};
}

#endif