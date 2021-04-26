#ifndef __UI_SCENE_HPP__
#define __UI_SCENE_HPP__


#include "UIwidgetBases.hpp"

#include <vector>


namespace Cookie::UI
{
	class UIscene
	{
		std::vector<UIwidget::GameWindowBase> sceneWidgets;

	public:
		void UpdateUI();

		inline std::vector<UIwidget::GameWindowBase>& EditWidgets()
		{ return sceneWidgets; }
	};
}



#endif // !__UI_SCENE_HPP__
