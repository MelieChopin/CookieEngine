#ifndef __UI_MENU_HPP__
#define __UI_MENU_HPP__

#include <functional>

namespace Cookie::Core		{ class Window; }
namespace Cookie::Resources { class ResourcesManager; }


namespace Cookie::UI
{
	class UImenu
	{
		const Cookie::Core::Window& window;
		Cookie::Resources::ResourcesManager& resources;

		mutable bool openMenuFlag = false;

		std::function<void()> quitFunc;

	public:
		mutable bool menuState = false;

	public:
		inline UImenu(const Cookie::Core::Window& _window, Cookie::Resources::ResourcesManager& _resources)
			: window	(_window),
			  resources	(_resources)
		{}
		

		inline void GiveQuitFunction(const std::function<void()>& _quitFunc)
		{ quitFunc = _quitFunc; }

		// This tells to open the associated menu. Once opened, the associated display function will make it appear, and only the user can close it.
		// The returned bool reference turns to false when the menu is closed.
		inline void SetMenuOpened()
		{ openMenuFlag |= true; }

		void DisplayMenus() const;
	};
}

#endif // !__UI_MENU_HPP__
