#ifndef	__COOKIEUI_WIDGET_BASES_HPP__
#define __COOKIEUI_WIDGET_BASES_HPP__


#include <imgui.h>

#include <GLFW/glfw3.h>

namespace Cookie::UIwidget
{
	struct WindowBase
	{
	protected:
		const char* windowName;
		bool		opened;

	public:
		WindowBase(const char* _windowName, bool _opened = true);

		virtual void WindowDisplay() = 0;
	};


	struct ItemBase
	{
	protected:
		const char* itemName;
		const char* shortcutSeq;
		bool		visible;


	public:
		ItemBase(const char* _itemName,							bool _visible = true);
		ItemBase(const char* _itemName, const char* _shortcutSeq, bool _visible = true);


		virtual void ItemDisplay();
	};


	struct WItemBase : WindowBase, ItemBase
	{
	protected:
		inline void Flip()
		{opened = !opened; visible = !visible;}


	public:

		/*
			Most basic contructor for the WItem, providing a name used by both Window and Item form and indicating what form it starts in.

			@param _name		Name of the WItem
			@param _isItemFirst (True if not indicated otherwise) Indicates if the WItem should begin as an item
		*/
		WItemBase(const char* _name, bool _isItemFirst = true)
			:	WindowBase	(_name, !_isItemFirst),
				ItemBase	(_name,  _isItemFirst)
		{}


		/*
			Contructor for the WItem, providing a name for the Window and the Item forms and indicating what form it starts in.

			@param _windowName	Name of the Window form
			@param _itemName	Name of the Item form
			@param _isItemFirst (True if not indicated otherwise) Indicates if the WItem should begin as an item
		*/
		WItemBase(const char* _windowName, const char* _itemName, bool _isItemFirst = true)
			:	WindowBase	(_windowName, !_isItemFirst),
				ItemBase	(_itemName,    _isItemFirst)
		{}


		/*
			Advanced contructor for the WItem, providing a name for the Window and the Item forms and indicating what form it starts in, with the additional mention of a usable shortcut.

			@param _windowName	Name of the Window form
			@param _itemName	Name of the Item form
			@param _shortcutSeq	Shortcut sequence to trigger the item remotely
			@param _isItemFirst (True if not indicated otherwise) Indicates if the WItem should begin as an item
		*/
		WItemBase(const char* _windowName, const char* _itemName, const char* _shortcutSeq, bool _isItemFirst = true)
			:	WindowBase	(_windowName,			   !_isItemFirst),
				ItemBase	(_itemName,	  _shortcutSeq, _isItemFirst)
		{}


		virtual void ItemDisplay() override;
	};
}

#endif