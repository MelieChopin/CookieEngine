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
    if (selectedEntity != recordedEntity)
    {
        recordedEntity = selectedEntity;
        selectedCollider = nullptr;
    }

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
        
        // due to the nature of how we can access and set variables in react physic 3D, this variable are used numerous times to ease the manipulations with ImGui and save memory.
        static float multiUseFloat = 0; static bool multiUseBool = false;


        if (TreeNode("Colliders"))
        {
            Text("Active colliders:");
            Indent(15.f);

            std::vector<reactphysics3d::Collider*>& colliders = physicComp.physColliders;
            
//====== COLLIDERS - beginning list of current colliders ======//

            for (size_t i = 0; i < colliders.size(); i++)
            {
                std::string nameTag;
            
                if (selectedCollider == colliders[i])
                    nameTag += '[';
            
                switch (colliders[i]->getCollisionShape()->getName())
                {
                case reactphysics3d::CollisionShapeName::SPHERE:    nameTag += "Sphere";    break;
                case reactphysics3d::CollisionShapeName::CAPSULE:   nameTag += "Capsule";   break;
                case reactphysics3d::CollisionShapeName::BOX:       nameTag += "Box";       break;

                default: break;
                } 

                if (selectedCollider == colliders[i])
                    nameTag += ']';

                nameTag += "##";
                nameTag += std::to_string(i);


                if (Button(nameTag.c_str()))
                    selectedCollider = colliders[i];
            }

            Unindent(15.f);

//====== COLLIDERS - sub-window for editing ======//
            
            if (BeginChild(windowName, { GetContentRegionAvail().x, 150 }, true) && (selectedCollider != nullptr))
            {
                multiUseBool = selectedCollider->getIsTrigger();
                if (Checkbox("Trigger:", &multiUseBool))
                    selectedCollider->setIsTrigger(multiUseBool);


                if (TreeNode("Collider transform"))
                {
                    reactphysics3d::Vector3     pos = selectedCollider->getLocalToBodyTransform().getPosition();
                    reactphysics3d::Quaternion  quat = selectedCollider->getLocalToBodyTransform().getOrientation();
                    Core::Math::Vec3            rot = Core::Math::Quat::ToEulerAngle({ quat.w, quat.x, quat.y, quat.z });

                    Text("Pos:"); SameLine(60.f);
                    if (DragFloat3("##COLPOS", &pos[0]))
                        selectedCollider->setLocalToBodyTransform({ pos, quat });

                    Text("Rot:"); SameLine(60.f);
                    if (DragFloat3("##COLROT", rot.e))
                        selectedCollider->setLocalToBodyTransform({ pos, reactphysics3d::Quaternion::fromEulerAngles(Core::Math::ToRadians(rot.x), Core::Math::ToRadians(rot.y), Core::Math::ToRadians(rot.z)) });

                    TreePop();
                }

                if (TreeNode("Collider material"))
                {
                    reactphysics3d::Material& colM = selectedCollider->getMaterial();

                    multiUseFloat = colM.getBounciness();
                    if (SliderFloat("##BOUNCINESS", &multiUseFloat, 0, 1, "Bounciness: %.5f"))
                        colM.setBounciness(multiUseFloat);

                    multiUseFloat = colM.getFrictionCoefficient();
                    if (DragFloat("##FRICTION", &multiUseFloat, 0.5, 0, 0, "Friction coef: %.3f"))
                        colM.setFrictionCoefficient(multiUseFloat >= 0 ? multiUseFloat : 0);

                    multiUseFloat = colM.getMassDensity();
                    if (DragFloat("##DENSITY", &multiUseFloat, 0.5, 0, 0, "Density: %.2f"))
                        colM.setMassDensity(multiUseFloat);

                    multiUseFloat = colM.getRollingResistance();
                    if (DragFloat("##ROLLRESIST", &multiUseFloat, 0.5, 0, 0, "Rolling resistance: %.5f"))
                        colM.setRollingResistance(multiUseFloat >= 0 ? multiUseFloat : 0);

                    TreePop();
                }
            }
            
            EndChild();

//====== COLLIDERS - options to add new colliders ======//

            if (Button("Add a collider")) OpenPopup("Collider adder popup");
            if (BeginPopup("Collider adder popup"))
            {
                if (Button("Add a sphere collider"))
                { 
                    physicComp.AddSphereCollider(physSim.GetSharedCommon(), 1, {0, 0, 0}, {0, 0, 0});
                
                    selectedCollider = colliders.back();
                    CloseCurrentPopup();
                }

                if (Button("Add a box collider"))
                {
                    physicComp.AddCubeCollider(physSim.GetSharedCommon(), {1, 1, 1}, { 0, 0, 0 }, { 0, 0, 0 });
                
                    selectedCollider = colliders.back();
                    CloseCurrentPopup();
                }

                if (Button("Add a capsule collider"))
                {
                    physicComp.AddCapsuleCollider(physSim.GetSharedCommon(), {1, 1}, { 0, 0, 0 }, { 0, 0, 0 });
                
                    selectedCollider = colliders.back();
                    CloseCurrentPopup();
                }

                EndPopup();
            }


            TreePop();
        }
        

        if (TreeNode("Rigibody"))
        {
            reactphysics3d::RigidBody*& rigibod = physicComp.physBody;

//====== RIGIBODY - all editable options ======//

            Text("Current entity dynamism:"); SameLine();

            switch (rigibod->getType())
            {
            case reactphysics3d::BodyType::STATIC:      TextColored({ 0.50, 0.50, 0.50, 1 }, "Static");     break;
            case reactphysics3d::BodyType::KINEMATIC:   TextColored({ 0.75, 0.75, 0.75, 1 }, "Kinematic");  break;
            case reactphysics3d::BodyType::DYNAMIC:     TextColored({ 1.00, 1.00, 1.00, 1 }, "Dynamic");    break;
            }

            Text("Change to:"); 
            
            SameLine();
            if (Button("Static")    && rigibod->getType() != reactphysics3d::BodyType::STATIC)      rigibod->setType(reactphysics3d::BodyType::STATIC);
            SameLine();
            if (Button("Kinematic") && rigibod->getType() != reactphysics3d::BodyType::KINEMATIC)   rigibod->setType(reactphysics3d::BodyType::KINEMATIC);
            SameLine();
            if (Button("Dynamic")   && rigibod->getType() != reactphysics3d::BodyType::DYNAMIC)     rigibod->setType(reactphysics3d::BodyType::DYNAMIC);

            NewLine();

            multiUseFloat = rigibod->getAngularDamping();
            if (DragFloat("##ANGULARDAMP", &multiUseFloat, 0.5, 0, 0, "Angular damping: %.3f"))
                rigibod->setAngularDamping(multiUseFloat >= 0 ? multiUseFloat : 0);

            multiUseFloat = rigibod->getLinearDamping();
            if (DragFloat("##LINEARDAMP", &multiUseFloat, 0.5, 0, 0, "Linear damping: %.3f"))
                rigibod->setLinearDamping(multiUseFloat >= 0 ? multiUseFloat : 0);

            multiUseFloat = rigibod->getMass();
            if (DragFloat("##MASS", &multiUseFloat, 1.0, 0, 0, "Mass: %.3f"))
                rigibod->setMass(multiUseFloat);
            

            multiUseBool = rigibod->isActive();
            if (Checkbox("Active", &multiUseBool))
                rigibod->setIsActive(multiUseBool);

            multiUseBool = rigibod->isAllowedToSleep();
            if (Checkbox("Allowed to sleep", &multiUseBool))
                rigibod->setIsAllowedToSleep(multiUseBool);
            
            multiUseBool = rigibod->isSleeping();
            Checkbox("Sleeping", &multiUseBool);
            
            multiUseBool = rigibod->isGravityEnabled();
            if (Checkbox("Gravity enabled", &multiUseBool))
                rigibod->enableGravity(multiUseBool);


            TreePop();
        }


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
