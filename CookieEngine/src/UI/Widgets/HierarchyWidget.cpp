#include "Coordinator.hpp"
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
        if (Button("Create empty"))
        { 
            coordinator.AddEntity(SIGNATURE_EMPTY);
            CloseCurrentPopup();
        }
        
        EndPopup();
    }


    EntityHandler& entityHandler = coordinator.entityHandler;
    for (size_t i = 0; i < entityHandler.livingEntities; i++)
    {
        if (Button((entityHandler.entities[i].name + "##" + std::to_string(i)).c_str()))
        {
            inspector->SelectEntity(&entityHandler.entities[i]);
        }
    }

    ImGui::End();
}