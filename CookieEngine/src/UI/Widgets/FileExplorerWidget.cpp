#include <string>
#include <filesystem>
#include "FileExplorerWidget.hpp"

#include <imgui.h>

namespace fs = std::filesystem;
using namespace ImGui;
using namespace Cookie::UIwidget;


void FileExplorer::ExploreFiles(const fs::path& path, const char* researchQuery)
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
                    Button(filename.filename().string().c_str());
            }
        }
    }
}

bool FileExplorer::HasReleventFile(const std::filesystem::path& folderPath, const std::string& researchQuery)
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
    if (ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_HorizontalScrollbar))
    {
        static char searchQuery[25]{ 0 };
        InputText("File search", &searchQuery[0], 25);

        ImGuiStyle& style = GetStyle();
        style.FrameRounding = 25.f;
        style.IndentSpacing = 30.0f;

        ExploreFiles(fs::current_path().parent_path().parent_path().parent_path(), &searchQuery[0]);

        style.FrameRounding = 0.f;
        style.IndentSpacing = 21.0f;
    }

    ImGui::End();
}