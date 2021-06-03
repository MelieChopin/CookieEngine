#ifndef __UI_SCENE_HPP__
#define __UI_SCENE_HPP__

#include "UIwidgetBases.hpp"

#include <vector>

namespace Cookie { class Game; }


namespace Cookie::UI
{
	class UIscene
	{
		std::vector<std::unique_ptr<UIwidget::GameWindowBase>> sceneWidgets;

		bool isHovered = false;
	
	public:
		int signature = 0;
	
	public:
		// Simple struct to facilitate UI scene load/save.
		struct GameWindowInfo
		{
			int ID;

			int xPos,  yPos,
				width, height;
		};

	
		UIscene() = default;
		UIscene(const std::vector<GameWindowInfo>& _gameWindows, Cookie::Game& _game);

		~UIscene();

		inline const bool IsHovered() { return isHovered; }

		void								LoadLayout	(const std::vector<GameWindowInfo>& GameWindows, Cookie::Game& game);
		const std::vector<GameWindowInfo>	SaveLayout	(bool clean = false);

		void CleanLayout();
		
		void RenderLayout();

		inline std::vector<std::unique_ptr<UIwidget::GameWindowBase>>& GetLayout()
		{ return sceneWidgets; }
	};
}

#endif // !__UI_SCENE_HPP__
