#ifndef	__FILE_EXPLORER_W_HPP__
#define __FILE_EXPLORER_W_HPP__

#include "UIwidgetBases.hpp"

namespace std				{ using string = basic_string<char, char_traits<char>, allocator<char>>; }
namespace std::filesystem	{ class path; }
namespace Cookie::Resources { class Texture; }
namespace Cookie::Render	{ class Renderer; }
namespace Cookie			{ class Game; }


namespace Cookie::UIwidget
{
	class FileExplorer final : public WItemBase
	{
		void ExploreFiles(const std::filesystem::path& path, const char* researchQuery)const;
		bool HasReleventFile(const std::filesystem::path& folderPath, const std::string& researchQuery)const;


		std::unique_ptr<Cookie::Resources::Texture> saveIcon;

		Cookie::Game& game;
	
	public:
		FileExplorer(Cookie::Render::Renderer& _renderer, Cookie::Game& _game);

		void WindowDisplay() override;
	};
}

#endif