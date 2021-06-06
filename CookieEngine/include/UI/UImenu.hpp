#ifndef __UI_MENU_HPP__
#define __UI_MENU_HPP__

namespace Cookie::Core		{ class Window; }
namespace Cookie::Resources { class ResourcesManager; }


namespace Cookie::UI
{
	class UImenu
	{
		const Cookie::Core::Window& window;
		Cookie::Resources::ResourcesManager& resources;

	public:
		enum class MenusID
		{
			MainMenu,
			EndMenu,

			MENU_COUNT
		};

	private:
		mutable bool openMenuFlags[(size_t)MenusID::MENU_COUNT] = {false};
		mutable bool menuState[(size_t)MenusID::MENU_COUNT] = {false};

	public:
		inline UImenu(const Cookie::Core::Window& _window, Cookie::Resources::ResourcesManager& _resources)
			: window	(_window),
			  resources	(_resources)
		{}
		

		// This tells to open the associated menu. Once opened, the associated display function will make it appear, and only the user can close it.
		// The returned bool reference turns to false when the menu is closed.
		inline bool& SetMenuOpened(MenusID menuId)
		{ 
			openMenuFlags[(size_t)menuId] |= true;
			return menuState[(size_t)menuId];
		}

		void DisplayMenus() const;
	};
}

#endif // !__UI_MENU_HPP__
