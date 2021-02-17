#ifndef	__COOKIEUI_WIDGETS_HPP__
#define __COOKIEUI_WIDGETS_HPP__


#include "UIwidgetBases.h"

#include <vector>
#include <string>


namespace std::filesystem {class path;};

namespace Cookie::UIwidget
{
	class Hierarchy : public WindowBase
	{
	public:
		Hierarchy()
			: WindowBase("Hierarchy")
		{}

		void WindowDisplay() override;
	};

	class FileExplorer : public WindowBase
	{
		void ExploreFiles(const std::filesystem::path& path, const char* researchQuery);
		bool HasReleventFile(const std::filesystem::path& folderPath, const ::std::string& researchQuery);

	public:
		FileExplorer()
			: WindowBase("File explorer")
		{}

		void WindowDisplay() override;
	};



	class ExitPannel : public WItemBase
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