#include "EntityHandler.hpp"
#include "InspectorWidget.hpp"
#include "HierarchyWidget.hpp"

#include <imgui.h>


using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;


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
        if (Button((entityHandler.entities[i].name + "##" + std::to_string(i)).c_str()))
        {
            inspector->SelectEntity(&entityHandler.entities[i]);
        }
    }

    ImGui::End();
}