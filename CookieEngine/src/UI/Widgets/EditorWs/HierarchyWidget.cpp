#include <vector>
#include "Resources/Scene.hpp"
#include "Coordinator.hpp"
#include "InspectorWidget.hpp"
#include "HierarchyWidget.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Editor.hpp"
#include "Renderer.hpp"

#include <imgui.h>

#include "Serialization.hpp"

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;


void Hierarchy::WindowDisplay()
{
    TryBeginWindow(ImGuiWindowFlags_HorizontalScrollbar)
    {
        if (BeginPopupContextWindow("Out-object hierarchy menu"))
        {
            if (Button("Create empty"))
            { 
                coordinator.AddEntity(SIGNATURE_EMPTY, resources);
                CloseCurrentPopup();
            }
        
            EndPopup();
        }


        TextColored({0.75, 0.75, 0.75, 1}, "%s", scene->name.c_str());


        Separator();

        EntityHandler& entityHandler = *coordinator.entityHandler;
        for (size_t i = 0; i < entityHandler.livingEntities; i++)
        {
            std::string entityNameTag = entityHandler.entities[i].name + "##" + std::to_string(i);

            if (Button(entityNameTag.c_str()))
            {
                selectedEntity.toChangeEntityId = entityHandler.entities[i].id;
            }

            if (BeginPopupContextItem(entityNameTag.c_str()))
            {
                if (Button("Delete entity"))
                {
                    coordinator.RemoveEntity(entityHandler.entities[i]);
                    CloseCurrentPopup();
                }

                if (Button("Save as prefab"))
                {
                    resources.CreateNewPrefabs(entityHandler.entities[i], *coordinator.componentHandler);
                    Cookie::Resources::Serialization::Save::SavePrefab(resources.prefabs[entityHandler.entities[i].namePrefab]);
                    CloseCurrentPopup();
                }
                
            
                EndPopup();
            }
        }
    }

    ImGui::End();
}