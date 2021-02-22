#include "Coordinator.hpp"
#include "Resources/ResourcesManager.hpp"
#include "InspectorWidget.hpp"

#include <imgui.h>
#include <imgui_stdlib.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;


void Inspector::WindowDisplay()
{
    ImGui::Begin(windowName, nullptr);

    if (selectedEntity)
    {
        InputText("Name", &selectedEntity->name);

        ImGui::NewLine();

        if (selectedEntity->signature & SIGNATURE_TRANSFORM)    TransformInterface();
        if (selectedEntity->signature & SIGNATURE_RIGIDBODY)    RigidBodyInterface();
        if (selectedEntity->signature & SIGNATURE_MODEL)        ModelCompInterface();


        if (Button("Add component...")) OpenPopup("Add component popup");

        if (BeginPopup("Add component popup"))
        {
            if (Button("Add component Transform"))  
            { 
                coordinator.componentHandler.AddComponentTransform  (*selectedEntity);
                CloseCurrentPopup();
            }
            
            if (Button("Add component RigidBody"))
            {
                coordinator.componentHandler.AddComponentRigidBody  (*selectedEntity);
                CloseCurrentPopup();
            }
            
            if (Button("Add component Model"))
            {
                coordinator.componentHandler.AddComponentModel      (*selectedEntity);
                CloseCurrentPopup();
            }

            EndPopup();
        }
    }

    ImGui::End();
}

void Inspector::TransformInterface()
{
    if (TreeNode("Transform"))
    {
        Transform& trsf = coordinator.componentHandler.GetComponentTransform(selectedEntity->id).localTRS;

        Text("Pos:"); DragFloat("X##POS", &trsf.translation.x); DragFloat("Y##POS", &trsf.translation.y); DragFloat("Z##POS", &trsf.translation.z);
        Text("Rot:"); DragFloat("X##ROT", &trsf.rotation.x);    DragFloat("Y##ROT", &trsf.rotation.y);    DragFloat("Z##ROT", &trsf.rotation.z);
        Text("Scl:"); DragFloat("X##SCL", &trsf.scale.x);       DragFloat("Y##SCL", &trsf.scale.y);       DragFloat("Z##SCL", &trsf.scale.z);


        ImGui::NewLine();
        if (Button("Remove component##TRSF"))
        {
            coordinator.componentHandler.RemoveComponentTransform(*selectedEntity);
        }

        TreePop();
    }

    ImGui::NewLine();
}

void Inspector::RigidBodyInterface()
{
    if (TreeNode("RigidBody"))
    {
        ComponentRigidBody& rigibod = coordinator.componentHandler.GetComponentRigidBody(selectedEntity->id);

        Text("Velocity:"); DragFloat("X##VEL", &rigibod.linearVelocity.x); DragFloat("Y##VEL", &rigibod.linearVelocity.y); DragFloat("Z##VEL", &rigibod.linearVelocity.z);

        Text("Mass:"); DragFloat("##MASS", &rigibod.mass);
        Text("Drag:"); DragFloat("##DRAG", &rigibod.drag);


        ImGui::NewLine();
        if (Button("Remove component##RIBOD"))
        {
            coordinator.componentHandler.RemoveComponentRigidBody(*selectedEntity);
        }

        TreePop();
    }

    ImGui::NewLine();
}

void Inspector::ModelCompInterface()
{
    if (TreeNode("Model"))
    {
        ComponentModel& modelComp = coordinator.componentHandler.GetComponentModel(selectedEntity->id);


//===== MESH PART =====//

        if (Button(modelComp.mesh != nullptr ? modelComp.mesh->name.c_str() : "No mesh applied##MESHCHECK")) OpenPopup("Mesh selector popup");

        if (BeginPopup("Mesh selector popup"))
        {
            for (const std::shared_ptr<Cookie::Resources::Mesh>& meshPtr : coordinator.resources.GetMeshes())
            {
                if (Button(meshPtr->name.c_str()))
                {
                    modelComp.mesh = meshPtr;
                    CloseCurrentPopup();
                    break;
                }
            }

            if (modelComp.mesh != nullptr && Button("Clear current mesh"))
            {
                modelComp.mesh.reset();
                CloseCurrentPopup();
            }

            EndPopup();
        }


//===== Shader part to come =====//
//===============================//

        ImGui::NewLine();
        if (Button("Remove component##MODEL"))
        {
            coordinator.componentHandler.RemoveComponentModel(*selectedEntity);
        }

        TreePop();
    }

    ImGui::NewLine();
}
