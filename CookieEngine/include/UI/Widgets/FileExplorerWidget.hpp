#ifndef	__FILE_EXPLORER_W_HPP__
#define __FILE_EXPLORER_W_HPP__

#include "UIwidgetBases.hpp"

namespace std				{ using string = basic_string<char, char_traits<char>, allocator<char>>; }
namespace std::filesystem	{ class path; }
namespace Cookie::Resources { class ResourcesManager; class Texture; }
namespace Cookie::Editor	{ class Scene; }
namespace Cookie::Render	{ class Renderer; }


namespace Cookie::UIwidget
{
	class FileExplorer final : public WItemBase
	{
		void ExploreFiles(const std::filesystem::path& path, const char* researchQuery)const;
		bool HasReleventFile(const std::filesystem::path& folderPath, const std::string& researchQuery)const;


		std::unique_ptr<Cookie::Resources::Texture> saveIcon;

		const Cookie::Resources::ResourcesManager& resources;
		Cookie::Editor::Scene& scene;
	
	public:
		FileExplorer(Cookie::Render::Renderer& _renderer, const Cookie::Resources::ResourcesManager& _resources, Cookie::Editor::Scene& _scene);

		void WindowDisplay() override;
	};
}

#endif