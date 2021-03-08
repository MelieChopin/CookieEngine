#ifndef __UI_CORE_HPP_
#define __UI_CORE_HPP_


#include "UIwidgetBases.hpp"

#include <vector>


struct GLFWwindow;

struct ID3D11DeviceContext;
struct ID3D11Device;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

namespace Cookie::Render {class Renderer;}


namespace Cookie::Core
{
	class UIcore
	{

		const std::vector<const char*> menus = {"File", "Edit", "Windows", "Help"};

		// All window-able elements of the engine, mostly WItems, which are positioned in the 4 first parts. All only-window elements are in the 5th spot.
		std::vector<UIwidget::WindowBase*>	UIndows[5];

		// All stored item elements of the engine UI.
		std::vector<UIwidget::ItemBase*>	UItems[4];

	private:
		void BeginFrame();
		void EndFrame();
	
	public:
		UIcore(GLFWwindow* _window, const Cookie::Render::Renderer& _renderer);
		void Terminate();


		// Adds a window inside the UI elements. Do mind, you'll need something to remote-control it if you intend to close it eventually.
		inline void AddWindow (UIwidget::WindowBase* UIW)
		{ UIndows[4].push_back(UIW); }

		// Adds an Item that'll be displayed in the main menu bar. They are usually not moving.
		inline void AddItem	(UIwidget::ItemBase* UIT, int i)
		{ UItems[i].push_back(UIT); }

		// WItems are considered both items and windows. Clicking the Item transforms it into a window.
		void AddWItem(UIwidget::WItemBase* UIWI, int i)
		{
			if (i <= 3)
			{
				UIndows[i].push_back(UIWI); 
				UItems [i].push_back(UIWI);
			}
		}


		/*
			Main rendering function for the overall UI

			Displays the main window, as well as the docking space with all other opened windows, and the menu bar.
		*/
		void UpdateUI();
	};
}

#endif