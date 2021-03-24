#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "Game.hpp"
#include "UI/UIeditor.hpp"
#include "Render/Camera.hpp"
#include "DebugRenderer.hpp"

namespace Cookie
{
	class Editor
	{
		private:
		public:
			Game					game;
			UI::UIeditor			ui;
			Render::FreeFlyCam		cam;
			Render::DebugRenderer	dbgRenderer;

			bool				isPlaying = false;
			
		private:
		public:
			Editor();
			~Editor();

			void Loop();
	};
}

#endif // !__EDITOR_HPP__
