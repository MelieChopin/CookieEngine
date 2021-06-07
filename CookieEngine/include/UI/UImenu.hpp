#ifndef __UI_MENU_HPP__
#define __UI_MENU_HPP__

#include <functional>

namespace Cookie::Core		{ class Window; }
namespace Cookie::Resources { class ResourcesManager; }
/* Inside ImGui.h */		  struct ImFont;

namespace Cookie::UI
{
	class UImenu
	{
		Cookie::Resources::ResourcesManager& resources;

		ImFont* tittleFont = nullptr;

		mutable bool openMenuFlag = false;

		std::function<void()> quitFunc;

	public:
		mutable bool menuState = false;

	public:
		UImenu(Cookie::Resources::ResourcesManager& _resources);
		
		inline void GiveQuitFunction(const std::function<void()>& _quitFunc)
		{ quitFunc = _quitFunc; }

		// This tells to open the associated menu. Once opened, the associated display function will make it appear, and only the user can close it.
		inline void SetMenuOpened()
		{ openMenuFlag |= true; }

		void DisplayMenus() const;
	};
}

#endif // !__UI_MENU_HPP__
