#include "Resources/ResourcesManager.hpp"
#include "Coordinator.hpp"
#include "Scene.hpp"
#include "InspectorWidget.hpp"

#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;
using namespace Cookie::Resources;


void Inspector::WindowDisplay()
{
    TryBeginWindow()
    {
        if      (selectedEntity)    EntityInspection();
        else if (selectedScene)     SceneInspection();
    }
    
    ImGui::End();
}


void Inspector::EntityInspection()
{
    InputText("Entity name", &selectedEntity->name);

    ImGui::Separator();

    if (selectedEntity->signature & SIGNATURE_TRANSFORM)    TransformInterface();
    if (selectedEntity->signature & SIGNATURE_RIGIDBODY)    RigidBodyInterface();
    if (selectedEntity->signature & SIGNATURE_MODEL)        ModelCompInterface();
    if (selectedEntity->signature & SIGNATURE_SCRIPT)       ScriptInterface();


    if (Button("Add component...")) OpenPopup("Add component popup");

    if (BeginPopup("Add component popup"))
    {
        if (Button("Add component Transform"))  
        { 
            coordinator.componentHandler->AddComponentTransform  (*selectedEntity);
            CloseCurrentPopup();
        }
            
        if (Button("Add component RigidBody"))
        {
            coordinator.componentHandler->AddComponentRigidBody  (*selectedEntity);
            CloseCurrentPopup();
        }
            
        if (Button("Add component Model"))
        {
            coordinator.componentHandler->AddComponentModel      (*selectedEntity);
            CloseCurrentPopup();
        }

        if (Button("Add component Script"))
        {
            coordinator.componentHandler->AddComponentScript     (*selectedEntity);
            CloseCurrentPopup();
        }

        EndPopup();
    }
}

void Inspector::TransformInterface()
{
    if (TreeNode("Transform"))
    {
        Transform& trsf = coordinator.componentHandler->GetComponentTransform(selectedEntity->id).localTRS;

        Text("Pos:"); SameLine(65.f); DragFloat3("##POS", trsf.translation.e);
        Text("Rot:"); SameLine(65.f); DragFloat3("##ROT", trsf.rotation.e);
        Text("Scl:"); SameLine(65.f); DragFloat3("##SCL", trsf.scale.e);

        ImGui::NewLine();
        if (Button("Remove component##TRSF"))
        {
            coordinator.componentHandler->RemoveComponentTransform(*selectedEntity);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::RigidBodyInterface()
{
    if (TreeNode("RigidBody"))
    {
        ComponentRigidBody& rigibod = coordinator.componentHandler->GetComponentRigidBody(selectedEntity->id);

        Text("Velocity:");          SameLine(110.f); DragFloat3("##VEL", rigibod.linearVelocity.e);

        Text("Mass:");              SameLine(110.f); DragFloat("##MASS", &rigibod.mass);
        Text("Drag:");              SameLine(110.f); DragFloat("##DRAG", &rigibod.drag);

        Text("TargetPos:");         SameLine(110.f); DragFloat3("##TargetPos", rigibod.targetPosition.e);
        
        Text("Speed:");             SameLine(110.f); DragFloat("##Speed", &rigibod.speed);
        
        Text("GoTowardPosition:");  SameLine(160.f); Checkbox("##GoTowardPosition", &rigibod.goTowardTarget);


        ImGui::NewLine();
        if (Button("Remove component##RIBOD"))
        {
            coordinator.componentHandler->RemoveComponentRigidBody(*selectedEntity);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::ModelCompInterface()
{
    if (TreeNode("Model"))
    {
        ComponentModel& modelComp = coordinator.componentHandler->GetComponentModel(selectedEntity->id);

//===== MESH =====//

        Text("Mesh:"); SameLine(100);

        if (Button(modelComp.mesh != nullptr ? modelComp.mesh->name.c_str() : "No mesh applied##MESHCHECK")) OpenPopup("Mesh selector popup");

        if (BeginPopup("Mesh selector popup"))
        {
            for (const std::shared_ptr<Cookie::Resources::Mesh>& meshPtr : resources.GetMeshes())
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

        
//===== SHADER =====//

//===== TEXTURE =====//
        
        Text("Texture:"); SameLine(100);

        if (Button(modelComp.texture != nullptr ? modelComp.texture->name.c_str() : "No texture applied##TEXTCHECK")) OpenPopup("Texture selector popup");

        if (BeginPopup("Texture selector popup"))
        {
            for (const std::shared_ptr<Cookie::Resources::Texture>& textPtr : resources.GetTextures())
            {
                if (Button(textPtr->name.c_str()))
                {
                    modelComp.texture = textPtr;
                    CloseCurrentPopup();
                    break;
                }

                if (IsItemHovered())
                {
                    BeginTooltip();
                    Image(static_cast<ImTextureID>(textPtr->GetResourceView()), {75, 75});
                    EndTooltip();
                }
            }

            if (modelComp.texture != nullptr && Button("Clear current texture"))
            {
                modelComp.texture.reset();
                CloseCurrentPopup();
            }

            EndPopup();
        }

//===================//

        ImGui::NewLine();
        if (Button("Remove component##MODEL"))
        {
            coordinator.componentHandler->RemoveComponentModel(*selectedEntity);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::ScriptInterface()
{
    if (TreeNode("Script"))
    {
        ComponentScript& script = coordinator.componentHandler->GetComponentScript(selectedEntity->id);

        for (int i = 0; i < script.scripts.size(); ++i)
        {
            Text("%s", script.scripts[i]->GetName());   
            if ( Button( ("Remove Script##" + i)) )
            { 
                script.scripts.erase(script.scripts.begin() + i); 
                i = (i > 0) ? i - 1 : 0;
            }
            ImGui::NewLine();
        }

        ImGui::NewLine();  if (Button("Add Script Hello World")) { script.scripts.push_back(new ScriptHelloWorld ); }
        ImGui::NewLine();  if (Button("Add Warning each 2s"))    { script.scripts.push_back(new ScriptWarning ); }

        ImGui::NewLine();
        if (Button("Remove component##RIBOD"))
        {
            coordinator.componentHandler->RemoveComponentScript(*selectedEntity);
        }

        TreePop();
    }

    ImGui::Separator();
}


void Inspector::SceneInspection()
{
    InputText("Scene name", &selectedScene->name);

    ImGui::Separator();
}