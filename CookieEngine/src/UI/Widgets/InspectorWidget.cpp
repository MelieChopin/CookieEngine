#include "Coordinator.hpp"
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

        NewLine();

        if (selectedEntity->signature & SIGNATURE_TRANSFORM)
        {
            if (TreeNode("Transform"))
            {
                Transform& trsf = coordinator.componentHandler.GetComponentTransform(selectedEntity->id).localTRS;

                Text("Pos:"); DragFloat("X##POS", &trsf.translation.x); DragFloat("Y##POS", &trsf.translation.y); DragFloat("Z##POS", &trsf.translation.z);
                Text("Rot:"); DragFloat("X##ROT", &trsf.rotation.x);    DragFloat("Y##ROT", &trsf.rotation.y);    DragFloat("Z##ROT", &trsf.rotation.z);
                Text("Scl:"); DragFloat("X##SCL", &trsf.scale.x);       DragFloat("Y##SCL", &trsf.scale.y);       DragFloat("Z##SCL", &trsf.scale.z);


                NewLine();
                if (Button("Remove component##TRSF"))
                { coordinator.componentHandler.RemoveComponentTransform(*selectedEntity); }

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


                NewLine();
                if (Button("Remove component##RIBOD"))
                { coordinator.componentHandler.RemoveComponentRigidBody(*selectedEntity); }

                TreePop();
            }

            NewLine();
        }

        if (selectedEntity->signature & SIGNATURE_MODEL)
        {
            if (TreeNode("Model"))
            {
                Text("[WIP]");


                NewLine();
                if (Button("Remove component##MODEL"))
                { coordinator.componentHandler.RemoveComponentModel(*selectedEntity); }

                TreePop();
            }

            NewLine();
        }


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