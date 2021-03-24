#include "Resources/ResourcesManager.hpp"
#include "Coordinator.hpp"
#include "Resources/Scene.hpp"
#include "InspectorWidget.hpp"

#include <string>
#include <imgui.h>
#include <imgui_stdlib.h>

#include <reactphysics3d.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;
using namespace Cookie::Resources;


void Inspector::WindowDisplay()
{
    TryBeginWindow()
    {
        if (selectedEntity) EntityInspection();
    }
    
    ImGui::End();
}


void Inspector::EntityInspection()
{
    InputText("Entity name", &selectedEntity->name);

    ImGui::Separator();

    if (selectedEntity->signature & SIGNATURE_TRANSFORM)    TransformInterface();
    if (selectedEntity->signature & SIGNATURE_MODEL)        ModelInterface();
    if (selectedEntity->signature & SIGNATURE_PHYSICS)      PhysicsInterface();
    if (selectedEntity->signature & SIGNATURE_SCRIPT)       ScriptInterface();
    //if (selectedEntity->signature & SIGNATURE_MAP)          MapInterface();

    if (Button("Add component...")) OpenPopup("Add component popup");

    if (BeginPopup("Add component popup"))
    {
        if  (selectedEntity->signature & SIGNATURE_TRANSFORM) TextDisabled("Component Transform already added");
        else if (Button("Add component Transform"))
        { 
            coordinator.componentHandler->AddComponentTransform (*selectedEntity);
            CloseCurrentPopup();
        }

        if (selectedEntity->signature & SIGNATURE_MODEL) TextDisabled("Component Model already added");
        else if (Button("Add component Model"))
        {
            coordinator.componentHandler->AddComponentModel     (*selectedEntity);
            CloseCurrentPopup();
        }
        
        if (selectedEntity->signature & SIGNATURE_PHYSICS) TextDisabled("Component Physics already added");
        else if (Button("Add component Physics"))
        {
            coordinator.componentHandler->AddComponentPhysics   (*selectedEntity, physSim);
            CloseCurrentPopup();
        }

        if (selectedEntity->signature & SIGNATURE_SCRIPT) TextDisabled("Component Script already added");
        else if (Button("Add component Script"))
        {
            coordinator.componentHandler->AddComponentScript    (*selectedEntity);
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
            for (std::unordered_map<std::string, std::shared_ptr<Mesh>>::iterator meshIt = resources.meshes.begin(); meshIt != resources.meshes.end(); meshIt++)
            {
                if (Button(meshIt->second->name.c_str()))
                {
                    modelComp.mesh = meshIt->second;
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
            for (std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator texIt = resources.textures.begin(); texIt != resources.textures.end(); texIt++)
            {
                if (Button(texIt->second->name.c_str()))
                {
                    modelComp.texture = texIt->second;
                    CloseCurrentPopup();
                    break;
                }

                if (IsItemHovered())
                {
                    BeginTooltip();
                    Image(static_cast<ImTextureID>(texIt->second->GetResourceView()), {75, 75});
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

void Inspector::PhysicsInterface()
{
    if (TreeNode("Physics"))
    {
        ComponentPhysics& physicComp = coordinator.componentHandler->GetComponentPhysics(selectedEntity->id);


        ImGui::NewLine();
        if (Button("Remove component##COLLIDER"))
        {
            coordinator.componentHandler->RemoveComponentPhysics(*selectedEntity);
        }

        TreePop();
    }
}

void Inspector::ScriptInterface()
{
    if (TreeNode("Script"))
    {
        ComponentScript& scriptC = coordinator.componentHandler->GetComponentScript(selectedEntity->id);
        
        for (size_t i = 0; i < scriptC.scripts.size();)
        { 
            Text("%s", scriptC.scripts[i].script->filename.c_str());
            Indent();
            
            if (Button( ("Remove##SCRIPT_" + std::to_string(i)).c_str() ))
            {
                scriptC.scripts.erase(scriptC.scripts.begin() + i);
            }
            else ++i;

            NewLine();
            Unindent();
        }


        if (Button("Add a script")) OpenPopup("Script selector popup");
        
        if (BeginPopup("Script selector popup"))
        {
            for (std::unordered_map<std::string, std::shared_ptr<Script>>::iterator scrIt = resources.scripts.begin(); scrIt != resources.scripts.end(); scrIt++)
            {
                if (Button(scrIt->second->filename.c_str()))
                {
                    scriptC.scripts.push_back(scrIt->second->CreateObject(std::to_string(selectedEntity->id)));
                    CloseCurrentPopup();
                }
            }

            EndPopup();
        }


        NewLine();
        if (Button("Remove component##SCRIPT"))
        {
            coordinator.componentHandler->RemoveComponentScript(*selectedEntity);
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


/*void Inspector::SceneInspection()
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
}*/
