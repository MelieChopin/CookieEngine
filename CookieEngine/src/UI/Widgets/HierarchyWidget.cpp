#include <vector>
#include "Resources/Scene.hpp"
#include "Coordinator.hpp"
#include "InspectorWidget.hpp"
#include "HierarchyWidget.hpp"

#include <imgui.h>

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


        //for (size_t i = 0; i < scenes->size(); i++)
        //{
            Resources::Scene& _scene = *scenes;

            std::string SceneNameTag;


            if (&_scene.entityHandler == coordinator.entityHandler)
            { SceneNameTag += "> "; }

            SceneNameTag += _scene.name + "##" + std::to_string('0');
        

            if (Button(SceneNameTag.c_str()))
            { inspector->SelectScene(scenes); }

            if (BeginPopupContextItem(SceneNameTag.c_str()))
            {
                if (Button("Load scene"))
                {
                    _scene.InitCoordinator(coordinator);
                    CloseCurrentPopup();
                }

                EndPopup();
            }
        //}

        Separator();

        EntityHandler& entityHandler = *coordinator.entityHandler;
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
    }

    ImGui::End();
}