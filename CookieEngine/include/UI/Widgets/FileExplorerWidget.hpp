#ifndef	__FILE_EXPLORER_W_HPP__
#define __FILE_EXPLORER_W_HPP__

#include "UIwidgetBases.h"


namespace std				{using string = basic_string<char, char_traits<char>, allocator<char>>;}
namespace std::filesystem	{class path;}


namespace Cookie::UIwidget
{
	class FileExplorer final : public WindowBase
	{
		void ExploreFiles(const std::filesystem::path& path, const char* researchQuery);
		bool HasReleventFile(const std::filesystem::path& folderPath, const std::string& researchQuery);

	public:
		FileExplorer()
			: WindowBase("File explorer")
		{}

		void WindowDisplay() override;
	};
}

#endif