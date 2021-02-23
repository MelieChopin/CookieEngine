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


    if (BeginPopupContextWindow("Out-object hierarchy menu"))
    {
        if (Button("Create empty"))
        { 
            coordinator.AddEntity(SIGNATURE_EMPTY, resources);
            CloseCurrentPopup();
        }
        
        EndPopup();
    }


    EntityHandler& entityHandler = coordinator.entityHandler;
    for (size_t i = 0; i < entityHandler.livingEntities; i++)
    {
        std::string entityNameTag = entityHandler.entities[i].name + "##" + std::to_string(i);

        if (Button(entityNameTag.c_str()))
        {
            inspector->SelectEntity(&entityHandler.entities[i]);
        }

        if (BeginPopupContextItem(entityNameTag.c_str()))
        {
            if (Button("Delete entity"))
            {
                coordinator.RemoveEntity(entityHandler.entities[i]);
                CloseCurrentPopup();
            }
            
            EndPopup();
        }
    }

    ImGui::End();
}