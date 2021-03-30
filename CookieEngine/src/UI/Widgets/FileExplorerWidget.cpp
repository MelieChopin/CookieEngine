#include <string>
#include <filesystem>
#include "Renderer.hpp"
#include "Resources/Texture.hpp"
#include "Game.hpp"
#include "FileExplorerWidget.hpp"

#include <imgui.h>
#include "Serialization.hpp"

#include "CustomImWidget.hpp"

namespace fs = std::filesystem;
using namespace ImGui;
using namespace Cookie::UIwidget;


FileExplorer::FileExplorer(Cookie::Render::Renderer& _renderer, Cookie::Game& _game, Cookie::ECS::Entity*& _selectedEntity)
            : WItemBase     ("File explorer"),
              game          (_game),
              selectedEntity(_selectedEntity)
{
    saveIcon = std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/Save2.ico");
}

void FileExplorer::ExploreFiles(const fs::path& path, const char* researchQuery)const
{
    if (fs::exists(path) && fs::is_directory(path))
    {
        for (const fs::directory_entry& entry : fs::directory_iterator(path))
        {
            const fs::path& filename = entry.path().c_str();
            if (fs::is_directory(entry.status()) && HasReleventFile(entry, researchQuery))
            {
                if (TreeNode(filename.filename().string().c_str()))
                {
                    ExploreFiles(entry, researchQuery);
                    TreePop();
                }
            }
        }


        for (const fs::directory_entry& entry : fs::directory_iterator(path))
        {
            const fs::path& filename = entry.path().c_str();
            if (fs::is_regular_file(entry.status()))
            {
                if (filename.string().find(researchQuery) != std::string::npos)
                {
                    if (filename.extension().string() == ".CAsset")
                    {
                        if (Custom::FileButton(filename.filename().string().c_str(), saveIcon->GetResourceView()))
                        {
                            selectedEntity = nullptr;
                            game.SetScene(Cookie::Resources::Serialization::Load::LoadScene(filename.string().c_str(), game));
                        }
                    }
                    else
                    {
                        Text("%s", filename.filename().string().c_str());
                    }
                }
            }
        }
    }
}

bool FileExplorer::HasReleventFile(const std::filesystem::path& folderPath, const std::string& researchQuery)const
{
    if (researchQuery.size() == 0) return true;

    for (const fs::directory_entry& entry : fs::directory_iterator(folderPath))
    {
        const fs::path& filename = entry.path().c_str();
        if (fs::is_directory(entry.status()))
        {
            if (HasReleventFile(entry, researchQuery)) return true;
        }
        else if (fs::is_regular_file(entry.status()))
        {
            if (filename.string().find(researchQuery) != std::string::npos) return true;
        }
    }

    return false;
}

void FileExplorer::WindowDisplay()
{
    TryBeginWindow(ImGuiWindowFlags_HorizontalScrollbar)
    {
        static char searchQuery[25]{ 0 };
        InputText("File search", &searchQuery[0], 25);


        PushStyleVar(ImGuiStyleVar_IndentSpacing, 35.f);

        ExploreFiles("Assets/", &searchQuery[0]);

        PopStyleVar();
    }

    ImGui::End();
}