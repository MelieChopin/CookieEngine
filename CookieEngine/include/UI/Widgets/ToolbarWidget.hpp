#ifndef	__TOOLBAR_W_HPP__
#define __TOOLBAR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class  Texture;	}
namespace Cookie::Render	{ class  Renderer;	}


namespace Cookie::UIwidget
{	
	enum class TransformTool
	{ Translate, Rotate, Scale, Quad };


	class Toolbar final : public WindowBase
	{
		enum class ToolbarIcons
		{ Translator, Rotator, Scaler, Quader, Save, Play, Stop, Pause, Frame, ICOS_NUM };

		std::unique_ptr<Cookie::Resources::Texture> icons[(int)ToolbarIcons::ICOS_NUM];
		
		TransformTool	currentTrsfTool	= TransformTool::Translate;
		bool			playing			= false;

	public:
		Toolbar(Cookie::Render::Renderer& _renderer);

		void WindowDisplay() final override;


		inline const TransformTool& CurrentTrsfTool()
		{ return currentTrsfTool; }

		inline const bool isPlaying()
		{ return playing; }
	};
}

#endif