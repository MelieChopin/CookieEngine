#ifndef __UI_SCENE_HPP__
#define __UI_SCENE_HPP__


#include "UIwidgetBases.hpp"

#include <vector>


namespace Cookie::UI
{
	class UIscene
	{
		std::vector<UIwidget::GameWindowBase*> sceneWidgets;

		
		// Simple struct to facilitate UI scene load/save.
		struct GameWindowInfo
		{
			int ID;

			int xPos,  yPos,
				width, height;
		};

	public:
		UIscene() = default;
		UIscene(const std::vector<GameWindowInfo>& _gameWindows);

		~UIscene();


		void								LoadLayout	(const std::vector<GameWindowInfo>& GameWindows);
		const std::vector<GameWindowInfo>	SaveLayout	(bool clean = false);

		void CleanLayout();
		
		inline std::vector<UIwidget::GameWindowBase*>& GetWidgets()
		{ return sceneWidgets; }
	};
}

#endif // !__UI_SCENE_HPP__
