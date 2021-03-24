#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "Game.hpp"
#include "UI/UIeditor.hpp"
#include "Render/Camera.hpp"

namespace Cookie
{
	namespace ECS
	{ class Entity; }

	class Editor
	{
		private:
		public:
			Game				game;
			UI::UIeditor		ui;
			Render::FreeFlyCam	cam;

			ECS::Entity*		selectedEntity = nullptr;

			bool				isPlaying = false;
			
		private:
		public:
			Editor();
			~Editor();

			void Loop();
	};
}

#endif // !__EDITOR_HPP__
