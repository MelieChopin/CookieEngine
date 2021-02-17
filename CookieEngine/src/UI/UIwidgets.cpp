#include "UI/UIwidgets.h"

#include "calc.hpp"

#include <filesystem>
#include <iostream>

namespace fs = ::std::filesystem;
using namespace ImGui;
using namespace Cookie::UIwidget;

void Hierarchy::WindowDisplay()
{
	ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    if (IsWindowHovered() && IsMouseClicked(ImGuiMouseButton_Right)) OpenPopup("Out-object hierarchy menu");

    if (BeginPopup("Out-object hierarchy menu"))
    {
        MenuItem("Yay! Success button!");
        MenuItem("More success button!");
        Text("Even a success text!");

        if (TreeNode("This is the magic success tree~!"))
        {
            if (TreeNode("Wait this goes deeper?"))
            {
                Text("Wow, this does go deepah!");
                TreePop();
            }

            Button("Boop me!~");

            TreePop();
        }

        EndPopup();
    }

	ImGui::End();
}

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
    ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_HorizontalScrollbar);

    static char searchQuery[25] {0};
    InputText("File search", &searchQuery[0], 25);

    ImGuiStyle& style = GetStyle();
    style.FrameRounding = 25.f;

    ExploreFiles(fs::current_path().parent_path().parent_path().parent_path(), &searchQuery[0]);

    style.FrameRounding = 0.f;

    ImGui::End();
}


void ExitPannel::WindowDisplay()
{
	if (!opened) return;


	ImGui::Begin(windowName, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);

	TextColored({ 1, 0, 0, 1 }, "Are you sure you want to close Cookie engine?");

	if (Button("Yes, close!", { 150, 50 })) glfwSetWindowShouldClose(window, true);
	SameLine(160.f);
	if (Button("Nooo, stay!", { 150, 50 })) Flip();

	ImGui::End();
}

