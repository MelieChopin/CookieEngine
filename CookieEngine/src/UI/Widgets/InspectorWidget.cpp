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
    if (selectedEntity->signature & SIGNATURE_MODEL)        ModelInterface();
    //if (selectedEntity->signature & SIGNATURE_MAP)          MapInterface();
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
        ComponentTransform& trsf = coordinator.componentHandler->GetComponentTransform(selectedEntity->id);

        Text("Pos:"); SameLine(65.f); DragFloat3("##POS", trsf.localTRS.pos.e);
        Text("Rot:"); SameLine(65.f); DragFloat3("##ROT", trsf.localTRS.rot.e);
        Text("Scl:"); SameLine(65.f); DragFloat3("##SCL", trsf.localTRS.scale.e);

        trsf.SetPhysics();

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

        //Text("Velocity:");          SameLine(110.f); DragFloat3("##VEL", rigibod.linearVelocity.e);
        //
        //Text("Mass:");              SameLine(110.f); DragFloat("##MASS", &rigibod.mass);
        //Text("Drag:");              SameLine(110.f); DragFloat("##DRAG", &rigibod.drag);
        //
        //Text("TargetPos:");         SameLine(110.f); DragFloat3("##TargetPos", rigibod.targetPosition.e);
        //
        //Text("Speed:");             SameLine(110.f); DragFloat("##Speed", &rigibod.speed);
        //
        //Text("GoTowardPosition:");  SameLine(160.f); Checkbox("##GoTowardPosition", &rigibod.goTowardTarget);


        ImGui::NewLine();
        if (Button("Remove component##RIBOD"))
        {
            coordinator.componentHandler->RemoveComponentRigidBody(*selectedEntity);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::ModelInterface()
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

void Inspector::MapInterface()
{
    if (TreeNode("Map transform"))
    {
        Transform& trsf = coordinator.componentHandler->GetComponentTransform(selectedEntity->id).localTRS;

        Text("Pos:"); SameLine(65.f); DragFloat3("##POS", trsf.pos.e);
        
        Text("Scl:"); SameLine(65.f); 
        if (DragFloat3("##SCL", trsf.scale.e)) CDebug.Log("Recoded an edit");

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::ScriptInterface()
{
    if (TreeNode("Script"))
    {
        ComponentScript& cScript = coordinator.componentHandler->GetComponentScript(selectedEntity->id);
        
        for (int i = 0; i < cScript.scripts.size(); ++i)
        { 
            Text("%s", cScript.scripts[i].script->filename.c_str() );
            std::string buttonText("Remove Script##");
            buttonText += i;
            if (Button(buttonText.c_str() ))
            {
                cScript.scripts.erase(cScript.scripts.begin() + i);
                i = (i > 0) ? i - 1 : 0;
            }
            ImGui::NewLine();
        }

        for (int i = 0; i < resources.scripts.size(); ++i)
        {
            ImGui::NewLine();  
            std::string buttonText("Add Script##");
            buttonText += i;
            if (Button(buttonText.c_str() ))
                {cScript.scripts.push_back(resources.scripts[i]->CreateObject(std::to_string(selectedEntity->id))); }
        }


        ImGui::NewLine();
        if (Button("Remove component##RIBOD"))
        {
            coordinator.componentHandler->RemoveComponentScript(*selectedEntity);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::SelectScene(Cookie::Editor::Scene* newSelection)
{
    selectedScene   = newSelection;
    selectedEntity  = nullptr;

    sceneTiles.x = (float)selectedScene->tiles.widthTile, 
    sceneTiles.y = (float)selectedScene->tiles.depthTile;
}

void Inspector::SceneInspection()
{
    InputText("Scene name", &selectedScene->name);

    ImGui::Separator();

    if (TreeNode("Tiles"))
    {
        Text("Current tiles: %d in x, %d in z", selectedScene->tiles.widthTile, selectedScene->tiles.depthTile);

        DragFloat2("##TILESNUM_EDIT", sceneTiles.e);

        if (sceneTiles.x != (float)selectedScene->tiles.widthTile ||
            sceneTiles.y != (float)selectedScene->tiles.depthTile)
        {
            if (sceneTiles.x > 0 && sceneTiles.y > 0)
            {
                if (Button("Save new dimensions"))
                    selectedScene->ChangeNumberOfTiles(sceneTiles.x, sceneTiles.y); 
            }
            else
            { TextColored({0.70f, 0.4f, 0.4f, 1}, "Invalid new values"); }

            SameLine();

            if (Button("Discard"))
            {
                sceneTiles.x = (float)selectedScene->tiles.widthTile,
                sceneTiles.y = (float)selectedScene->tiles.depthTileProp;
            }
        }

        TreePop();
    }
}
