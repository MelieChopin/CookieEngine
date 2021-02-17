#ifndef	__COOKIEUI_WIDGETS_HPP__
#define __COOKIEUI_WIDGETS_HPP__


#include "UIwidgetBases.h"

#include <vector>
#include <string>


namespace std::filesystem	{class path;};

namespace Cookie::ECS
{
	class Coordinator;
	class EntityHandler; 
	class Entity;
}

namespace Cookie::UIwidget
{
	class Viewport final : public WindowBase
	{
	public:
		Viewport()
			: WindowBase("Viewport")
		{}

		void WindowDisplay() override;
	};
	
	class GamePort final : public WindowBase
	{
	public:
		GamePort()
			: WindowBase("Game")
		{}

		void WindowDisplay() override;
	};
	

	class Inspector final : public WindowBase
	{
		Cookie::ECS::Coordinator& coordinator;

		Cookie::ECS::Entity* selectedEntity = nullptr;

	public:
		Inspector(Cookie::ECS::Coordinator& _coordinator)
			: WindowBase	("Inspector"),
			  coordinator	(_coordinator)
		{}

		void WindowDisplay() override;

		inline void SelectEntity(Cookie::ECS::Entity* newSelection)
		{selectedEntity = newSelection;}
	};


	class Hierarchy final : public WindowBase
	{
		Cookie::ECS::EntityHandler& entityHandler;
		Inspector*					inspector;

	public:
		Hierarchy(Cookie::ECS::EntityHandler& _entityHandler, Inspector* _inspector)
			: WindowBase	("Hierarchy"),
			  entityHandler	(_entityHandler),
			  inspector		(_inspector)
		{}

		void WindowDisplay() override;
	};


	class FileExplorer final : public WindowBase
	{
		void ExploreFiles(const std::filesystem::path& path, const char* researchQuery);
		bool HasReleventFile(const std::filesystem::path& folderPath, const ::std::string& researchQuery);

	public:
		FileExplorer()
			: WindowBase("File explorer")
		{}

		void WindowDisplay() override;
	};



	class ExitPannel final : public WItemBase
	{
		GLFWwindow* window;

	public:
		ExitPannel(GLFWwindow* _window)
			: window		(_window),
			  WItemBase	("Exit confirmation", "Exit", "Esc")
		{}

		void WindowDisplay() override;
	};
}

#endif