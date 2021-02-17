#include "Coordinator.hpp"
#include "UI/UIwidgets.h"

#include <filesystem>
#include <iostream>


namespace fs = ::std::filesystem;
using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;


void Viewport::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);
    ImGui::End();
}

void GamePort::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);
    ImGui::End();
}

void Inspector::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);

    if (selectedEntity)
    {
        if (selectedEntity->signature & SIGNATURE_TRANSFORM)
        {
            if (TreeNode("Transform"))
            {
                Transform& trsf = coordinator.componentHandler.GetComponentTransform(selectedEntity->id).localTRS;

                Text("Pos:"); DragFloat("X##POS", &trsf.translation.x); DragFloat("Y##POS", &trsf.translation.y); DragFloat("Z##POS", &trsf.translation.z);
                Text("Rot:"); DragFloat("X##ROT", &trsf.rotation.x);    DragFloat("Y##ROT", &trsf.rotation.y);    DragFloat("Z##ROT", &trsf.rotation.z);
                Text("Scl:"); DragFloat("X##SCL", &trsf.scale.x);       DragFloat("Y##SCL", &trsf.scale.y);       DragFloat("Z##SCL", &trsf.scale.z);
            
                TreePop();
            }

            NewLine();
        }

        if (selectedEntity->signature & SIGNATURE_RIGIDBODY)
        {
            if (TreeNode("RigidBody"))
            {
                ComponentRigidBody& rigibod = coordinator.componentHandler.GetComponentRigidBody(selectedEntity->id);

                Text("Velocity:"); DragFloat("X##VEL", &rigibod.linearVelocity.x); DragFloat("Y##VEL", &rigibod.linearVelocity.y); DragFloat("Z##VEL", &rigibod.linearVelocity.z);
            
                Text("Mass:"); DragFloat("##MASS", &rigibod.mass);
                Text("Drag:"); DragFloat("##DRAG", &rigibod.drag);

                TreePop();
            }

            NewLine();
        }

        if (selectedEntity->signature & SIGNATURE_MODEL)
        {
            if (TreeNode("Model"))
            {
                Text("[WIP]");
            }

            NewLine();
        }
    }

    ImGui::End();
}

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

    for (size_t i = 0; i < entityHandler.livingEntities; i++)
    {
        if (Button( (entityHandler.entities[i].name + "##" + std::to_string(i)).c_str()) )
        {
            inspector->SelectEntity(&entityHandler.entities[i]);
        }
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

