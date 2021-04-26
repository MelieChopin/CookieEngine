#ifndef __UI_GAME_HPP__
#define __UI_GAME_HPP__


#include "UIwidgetBases.hpp"

#include <vector>


struct ImGuiContext;


namespace Cookie::UI
{
	class UIscene;

	class UIgame
	{
		UIscene* currentScene;
		
	public:
		UIgame() = default;


		inline void SetScene (UIscene* incommingScene)
		{ currentScene = incommingScene; }
		
		
		// This will simply draw a global invisible window as a canvas, and display the current linked scene's UI elements/widgets on it as childs that can't be edited.
		void DrawSceneUI	();

		// Draws the scene's UI elements/widgets directly each as editable windows.
		void EditSceneUI	();
	};
}

#endif // !__UI_GAME_HPP__