#ifndef __VIEWPORT_W_HPP__
#define __VIEWPORT_W_HPP__

#include "UIwidgetBases.h"

struct GLFWwindow;
namespace Cookie::Core::Math	{ union Vec2; }
namespace Cookie::Render		{ class FrameBuffer; class Camera; }


namespace Cookie::UIwidget
{
	class Viewport final : public WindowBase
	{
		Cookie::Core::Math::Vec2 recordedWinSize;

		GLFWwindow* window;

		const Cookie::Render::FrameBuffer&	frameBuffer;
		std::shared_ptr<Cookie::Render::Camera>* camera;

	public:
		Viewport(GLFWwindow* _window, const Cookie::Render::FrameBuffer& _frameBuffer, std::shared_ptr<Cookie::Render::Camera>* _camera)
			: WindowBase	("Viewport"),
			  window		(_window),
			  frameBuffer	(_frameBuffer),
			  camera		(_camera)
		{
		}

		void WindowDisplay() override;
	};
}

#endif