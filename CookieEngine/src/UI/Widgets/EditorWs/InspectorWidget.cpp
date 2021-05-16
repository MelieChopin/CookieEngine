
#include <d3d11.h>
#include "Coordinator.hpp"
#include "Resources/Scene.hpp"
#include "ECS/ComponentModel.hpp"
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/ResourcesManager.hpp" 
#include "InspectorWidget.hpp"
#include "Editor.hpp"
#include <reactphysics3d.h>
#include "InspectorWidget.hpp"

#include <string>

#include <imgui.h>
#include <imgui_stdlib.h>
#include "CustomImWidget.hpp"

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;
using namespace Cookie::Resources;


void Inspector::WindowDisplay()
{
    if (selectedEntity.focusedEntity != recordedEntity || selectedEntity.componentHandler != coordinator.componentHandler)
    {
        recordedEntity = selectedEntity.focusedEntity;
        selectedCollider = nullptr;
    }

    TryBeginWindow()
    {
        if (selectedEntity.focusedEntity) EntityInspection();
    }
    
    ImGui::End();
}


void Inspector::EntityInspection()
{
    InputText("Entity name", &selectedEntity.focusedEntity->name);
    InputText("Entity tag",  &selectedEntity.focusedEntity->tag );

    ImGui::Separator();

    if (selectedEntity.focusedEntity->signature & C_SIGNATURE::TRANSFORM)    TransformInterface();
    if (selectedEntity.focusedEntity->signature & C_SIGNATURE::MODEL)        ModelInterface();
    if (selectedEntity.focusedEntity->signature & C_SIGNATURE::PHYSICS)      PhysicsInterface();
    if (selectedEntity.focusedEntity->signature & C_SIGNATURE::SCRIPT)       ScriptInterface();
    if (selectedEntity.focusedEntity->signature & C_SIGNATURE::GAMEPLAY)     GameplayInterface();


    if (Button("Add component...")) OpenPopup("Add component popup");

    if (BeginPopup("Add component popup"))
    {
        if (selectedEntity.focusedEntity->signature & C_SIGNATURE::TRANSFORM)  TextDisabled("Component Transform already added");
        else if (Selectable("Add component Transform"))
            coordinator.componentHandler->AddComponent(*selectedEntity.focusedEntity, C_SIGNATURE::TRANSFORM);


        if (selectedEntity.focusedEntity->signature & C_SIGNATURE::MODEL)      TextDisabled("Component Model already added");
        else if (Selectable("Add component Model"))
            coordinator.componentHandler->AddComponent(*selectedEntity.focusedEntity, C_SIGNATURE::MODEL);


        if (selectedEntity.focusedEntity->signature & C_SIGNATURE::PHYSICS)    TextDisabled("Component Physics already added");
        else if (Selectable("Add component Physics"))
        {
            coordinator.componentHandler->AddComponent(*selectedEntity.focusedEntity, C_SIGNATURE::PHYSICS);
            coordinator.componentHandler->GetComponentPhysics(selectedEntity.focusedEntity->id).Activate();
        }


        if (selectedEntity.focusedEntity->signature & C_SIGNATURE::SCRIPT)     TextDisabled("Component Script already added");
        else if (Selectable("Add component Script"))
            coordinator.componentHandler->AddComponent(*selectedEntity.focusedEntity, C_SIGNATURE::SCRIPT);


        if (selectedEntity.focusedEntity->signature & C_SIGNATURE::GAMEPLAY)   TextDisabled("Component Gameplay already added");
        else if (Selectable("Add component Gameplay"))
            coordinator.componentHandler->AddComponent(*selectedEntity.focusedEntity, C_SIGNATURE::GAMEPLAY);

        EndPopup();
    }
}

void Inspector::TransformInterface()
{
    if (TreeNode("Transform"))
    {
        ComponentTransform& trsf = coordinator.componentHandler->GetComponentTransform(selectedEntity.focusedEntity->id);

        Text("Pos:"); SameLine(65.f); bool posHasChanged =  DragFloat3("##POS", trsf.pos.e);
        Text("Rot:"); SameLine(65.f); bool rotHasChanged =  DragFloat3("##ROT", trsf.rot.e);
        Text("Scl:"); SameLine(65.f); bool sclHasChanged =  DragFloat3("##SCL", trsf.scale.e);

        if (posHasChanged || rotHasChanged || sclHasChanged)
            trsf.trsHasChanged = true;
        
        NewLine();
        if (Selectable("Remove component##TRSF"))
        {
            coordinator.componentHandler->RemoveComponent(*selectedEntity.focusedEntity, C_SIGNATURE::TRANSFORM);
        }


        TreePop();
    }

    ImGui::Separator();
}

void Inspector::ModelInterface()
{
    if (TreeNode("Model"))
    {
        ComponentModel& modelComp = coordinator.componentHandler->GetComponentModel(selectedEntity.focusedEntity->id);

        static std::string researchString;

        //===== MESH =====//

        Text("Mesh:"); SameLine(100);

        if (BeginCombo("##MESHSELECT", modelComp.mesh != nullptr ? modelComp.mesh->name.c_str() : "No mesh applied", ImGuiComboFlags_HeightLarge))
        {
            InputText("Mesh search", &researchString, ImGuiInputTextFlags_AutoSelectAll);

            NewLine();
            
            for (std::unordered_map<std::string, std::unique_ptr<Mesh>>::iterator meshIt = resources.meshes.begin(); meshIt != resources.meshes.end(); meshIt++)
            {
                const bool is_selected = (modelComp.mesh != nullptr && modelComp.mesh->name == meshIt->second->name);

                if ((meshIt->second->name.find(researchString) != std::string::npos) && ImGui::Selectable(meshIt->second->name.c_str(), is_selected))
                {
                    modelComp.mesh = meshIt->second.get();
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            NewLine();

            if (modelComp.mesh != nullptr)
            {
                if (Button("Clear current mesh"))
                    modelComp.mesh = nullptr;
            }
            else TextDisabled("Clear current mesh");

            EndCombo();
        }

        if (ImGui::IsItemClicked())
            researchString.clear();

//===== TEXTURE =====//
        
        Text("Albedo:"); SameLine(100);

        if (BeginCombo("##AlbedoSELECT", modelComp.albedo != nullptr ? modelComp.albedo->name.c_str() : "No texture applied", ImGuiComboFlags_HeightLarge))
        {
            InputText("Texture search", &researchString, ImGuiInputTextFlags_AutoSelectAll);

            NewLine();

            for (std::unordered_map<std::string, std::unique_ptr<Texture>>::iterator textIt = resources.textures.begin(); textIt != resources.textures.end(); textIt++)
            {
                const bool is_selected = (modelComp.albedo != nullptr && textIt->second &&  modelComp.albedo->name == textIt->second->name);

                if (textIt->second && textIt->second->name.find(researchString) != std::string::npos)
                {
                    if (textIt->second->desc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
                    {
                        Custom::Zoomimage(static_cast<ImTextureID>(textIt->second->GetResourceView()), 25, 25, 5);

                        SameLine();

                        if (ImGui::Selectable(textIt->second->name.c_str(), is_selected))
                            modelComp.albedo = textIt->second.get();
                    }
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            NewLine();

            if (modelComp.albedo != nullptr)
            {
                if (Button("Clear current texture"))
                    modelComp.albedo = nullptr;
            }
            else TextDisabled("Clear current texture");

            EndCombo();
        }

        Text("Normal:"); SameLine(100);

        if (BeginCombo("##NormSELECT", modelComp.normal != nullptr ? modelComp.normal->name.c_str() : "No texture applied", ImGuiComboFlags_HeightLarge))
        {
            InputText("Texture search", &researchString, ImGuiInputTextFlags_AutoSelectAll);

            NewLine();

            for (std::unordered_map<std::string, std::unique_ptr<Texture>>::iterator textIt = resources.textures.begin(); textIt != resources.textures.end(); textIt++)
            {
                const bool is_selected = (modelComp.normal != nullptr && textIt->second && modelComp.normal->name == textIt->second->name);

                if (textIt->second && textIt->second->name.find(researchString) != std::string::npos)
                {
                    if (textIt->second->desc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
                    {
                        Custom::Zoomimage(static_cast<ImTextureID>(textIt->second->GetResourceView()), 25, 25, 5);

                        SameLine();

                        if (ImGui::Selectable(textIt->second->name.c_str(), is_selected))
                            modelComp.normal = textIt->second.get();
                    }
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            NewLine();

            if (modelComp.normal != nullptr)
            {
                if (Button("Clear current texture"))
                    modelComp.normal = nullptr;
            }
            else TextDisabled("Clear current texture");

            EndCombo();
        }

        Text("Metallic-Roughness:"); SameLine(100);

        if (BeginCombo("##MRSELECT", modelComp.metallicRoughness != nullptr ? modelComp.metallicRoughness->name.c_str() : "No texture applied", ImGuiComboFlags_HeightLarge))
        {
            InputText("Texture search", &researchString, ImGuiInputTextFlags_AutoSelectAll);

            NewLine();

            for (std::unordered_map<std::string, std::unique_ptr<Texture>>::iterator textIt = resources.textures.begin(); textIt != resources.textures.end(); textIt++)
            {
                const bool is_selected = (modelComp.metallicRoughness != nullptr && textIt->second && modelComp.metallicRoughness->name == textIt->second->name);

                if (textIt->second && textIt->second->name.find(researchString) != std::string::npos)
                {
                    if (textIt->second->desc.ViewDimension == D3D11_SRV_DIMENSION_TEXTURE2D)
                    {
                        Custom::Zoomimage(static_cast<ImTextureID>(textIt->second->GetResourceView()), 25, 25, 5);

                        SameLine();

                        if (ImGui::Selectable(textIt->second->name.c_str(), is_selected))
                            modelComp.metallicRoughness = textIt->second.get();
                    }
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            NewLine();

            if (modelComp.metallicRoughness != nullptr)
            {
                if (Button("Clear current texture"))
                    modelComp.metallicRoughness = nullptr;
            }
            else TextDisabled("Clear current texture");

            EndCombo();
        }

        if (ImGui::IsItemClicked())
            researchString.clear();

        //===================//

        NewLine();
        if (Selectable("Remove component##MODEL"))
        {
            coordinator.componentHandler->RemoveComponent(*selectedEntity.focusedEntity, C_SIGNATURE::MODEL);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::PhysicsInterface()
{
    if (TreeNode("Physics"))
    {
        ComponentPhysics& physicComp = coordinator.componentHandler->GetComponentPhysics(selectedEntity.focusedEntity->id);

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
                if (Checkbox("Trigger", &multiUseBool))
                    selectedCollider->setIsTrigger(multiUseBool);


                if (TreeNode("Collider transform"))
                {
                    reactphysics3d::Vector3     pos = selectedCollider->getLocalToBodyTransform().getPosition();
                    reactphysics3d::Quaternion  quat = selectedCollider->getLocalToBodyTransform().getOrientation();
                    Core::Math::Vec3            rot = Core::Math::Quat::ToEulerAngle({ quat.w, quat.x, quat.y, quat.z });

                    Text("Pos:"); SameLine(60.f);
                    if (DragFloat3("##COLPOS", &pos[0], 0.25))
                        selectedCollider->setLocalToBodyTransform({ pos, quat });

                    Text("Rot:"); SameLine(60.f);
                    if (DragFloat3("##COLROT", rot.e))
                        selectedCollider->setLocalToBodyTransform({ pos, reactphysics3d::Quaternion::fromEulerAngles(Core::Math::ToRadians(rot.x), Core::Math::ToRadians(rot.y), Core::Math::ToRadians(rot.z)) });

                    NewLine();
                    Text("Characteristics:");

                    if (selectedCollider->getCollisionShape()->getName() == reactphysics3d::CollisionShapeName::SPHERE)
                    {
                        reactphysics3d::SphereShape* sphereCol = static_cast<reactphysics3d::SphereShape*>(selectedCollider->getCollisionShape());


                        multiUseFloat = sphereCol->getRadius();
                        if (DragFloat("##SRADIUS", &multiUseFloat, 0.25, 0, 0, "Radius: %.3f"))
                            sphereCol->setRadius(multiUseFloat > 0 ? multiUseFloat : 0.001);
                    }
                    else if (selectedCollider->getCollisionShape()->getName() == reactphysics3d::CollisionShapeName::CAPSULE)
                    {
                        reactphysics3d::CapsuleShape* capsuleCol = static_cast<reactphysics3d::CapsuleShape*>(selectedCollider->getCollisionShape());


                        multiUseFloat = capsuleCol->getHeight();
                        if (DragFloat("##CHEIGHT", &multiUseFloat, 0.25, 0, 0, "Height: %.3f"))
                            capsuleCol->setHeight(multiUseFloat > 0 ? multiUseFloat : 0.001);

                        multiUseFloat = capsuleCol->getRadius();
                        if (DragFloat("##CRADIUS", &multiUseFloat, 0.25, 0, 0, "Radius: %.3f"))
                            capsuleCol->setRadius(multiUseFloat > 0 ? multiUseFloat : 0.001);
                    }
                    else if (selectedCollider->getCollisionShape()->getName() == reactphysics3d::CollisionShapeName::BOX)
                    {
                        reactphysics3d::BoxShape* boxCol = static_cast<reactphysics3d::BoxShape*>(selectedCollider->getCollisionShape());


                        reactphysics3d::Vector3&& halfExtents = boxCol->getHalfExtents();
                        Text("Half extents:"); SameLine();
                        if (DragFloat3("##BSIZE", &halfExtents[0], 0.25))
                            boxCol->setHalfExtents(reactphysics3d::Vector3::max({ 0.001, 0.001, 0.001 }, halfExtents));
                    }

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
                    physicComp.AddSphereCollider(1, { 0, 0, 0 }, { 0, 0, 0 });

                    selectedCollider = colliders.back();
                    CloseCurrentPopup();
                }

                if (Button("Add a box collider"))
                {
                    physicComp.AddCubeCollider({ 1, 1, 1 }, { 0, 0, 0 }, { 0, 0, 0 });

                    selectedCollider = colliders.back();
                    CloseCurrentPopup();
                }

                if (Button("Add a capsule collider"))
                {
                    physicComp.AddCapsuleCollider({ {1, 1} }, { 0, 0, 0 }, { 0, 0, 0 });

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
            if (Button("Static") && rigibod->getType() != reactphysics3d::BodyType::STATIC)         rigibod->setType(reactphysics3d::BodyType::STATIC);
            SameLine();
            if (Button("Kinematic") && rigibod->getType() != reactphysics3d::BodyType::KINEMATIC)   rigibod->setType(reactphysics3d::BodyType::KINEMATIC);
            SameLine();
            if (Button("Dynamic") && rigibod->getType() != reactphysics3d::BodyType::DYNAMIC)       rigibod->setType(reactphysics3d::BodyType::DYNAMIC);

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


        NewLine();
        if (Selectable("Remove component##COLLIDER"))
        {
            coordinator.componentHandler->RemoveComponent(*selectedEntity.focusedEntity, C_SIGNATURE::PHYSICS);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::ScriptInterface()
{
    if (TreeNode("Script"))
    {
        ComponentScript& scriptComp = coordinator.componentHandler->GetComponentScript(selectedEntity.focusedEntity->id);
        
        for (size_t i = 0; i < scriptComp.scripts.size();)
        { 
            Text("%s", scriptComp.scripts[i].script->filename.c_str());
            Indent();
            
            if (Button( ("Remove##SCRIPT_" + std::to_string(i)).c_str() ))
            {
                scriptComp.scripts.erase(scriptComp.scripts.begin() + i);
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
                    scriptComp.scripts.push_back(scrIt->second->CreateObject(std::to_string(selectedEntity.focusedEntity->id)));
                    CloseCurrentPopup();
                }
            }

            EndPopup();
        }


        NewLine();
        if (Selectable("Remove component##SCRIPT"))
        {
            coordinator.componentHandler->RemoveComponent(*selectedEntity.focusedEntity, C_SIGNATURE::SCRIPT);
        }

        TreePop();
    }

    ImGui::Separator();
}

void Inspector::GameplayInterface()
{
    if (TreeNode("Gameplay properties"))
    {
        ComponentGameplay& gameplayComp = coordinator.componentHandler->GetComponentGameplay(selectedEntity.focusedEntity->id);

        if (gameplayComp.signatureGameplay & CGP_SIGNATURE::LIVE)
        {
            if (TreeNode("Life/Armor properties"))
            {
                DragFloat("##LIFE", &gameplayComp.componentLive.life, 1.f, NULL, NULL, "Life: %.0f");
                DragFloat("##ARMOR", &gameplayComp.componentLive.armor, 1.f, NULL, NULL, "Armor: %.0f");


                NewLine();
                if (Selectable("Remove property##LIVE"))
                {
                    gameplayComp.componentLive.ToDefault();
                    gameplayComp.signatureGameplay -= CGP_SIGNATURE::LIVE;
                }

                TreePop();
            }
        }
        else if (Selectable("Add life/Armor properties"))
            gameplayComp.signatureGameplay += CGP_SIGNATURE::LIVE;


        if (gameplayComp.signatureGameplay & CGP_SIGNATURE::MOVE)
        {
            if (TreeNode("Movement capacities"))
            {
                DragFloat("##SPEED", &gameplayComp.componentMove.moveSpeed, 0.25f, NULL, NULL, "Speed: %.2f");
                Text("Flying:"); SameLine(); Checkbox("##CANFLY", &gameplayComp.componentMove.isFlying);

                DragFloat("##RADIUS", &gameplayComp.componentMove.radius, 0.25f, NULL, NULL, "Radius: %.2f");


                NewLine();
                if (Selectable("Remove property##MOVE"))
                {
                    gameplayComp.componentMove.ToDefault();
                    gameplayComp.signatureGameplay -= CGP_SIGNATURE::MOVE;
                }

                TreePop();
            }
        }
        else if (Selectable("Add movement capacities"))
            gameplayComp.signatureGameplay += CGP_SIGNATURE::MOVE;


        if (gameplayComp.signatureGameplay & CGP_SIGNATURE::ATTACK)
        {
            if (TreeNode("Attack abilities"))
            {
                DragFloat("##DAMAGE", &gameplayComp.componentAttack.attackDamage, 0.25f, NULL, NULL, "Damage: %.2f");
                DragFloat("##RANGE", &gameplayComp.componentAttack.attackRange, 0.25f, NULL, NULL, "Range: %.2f");
                DragFloat("##SPEED", &gameplayComp.componentAttack.attackSpeed, 0.25f, NULL, NULL, "Speed: %.2f");


                NewLine();
                if (Selectable("Remove property##ATTACK"))
                {
                    gameplayComp.componentMove.ToDefault();
                    gameplayComp.signatureGameplay -= CGP_SIGNATURE::ATTACK;
                }

                TreePop();
            }
        }
        else if (Selectable("Add attack abilities"))
            gameplayComp.signatureGameplay += CGP_SIGNATURE::ATTACK;


        NewLine();
        if (Selectable("Remove component##GAMEPLAY"))
        {
            coordinator.componentHandler->RemoveComponent(*selectedEntity.focusedEntity, C_SIGNATURE::GAMEPLAY);
        }

        TreePop();
    }

    ImGui::Separator();
}

