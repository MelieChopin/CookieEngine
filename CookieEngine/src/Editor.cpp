#include "Editor.hpp" 
#include "UIallIn.hpp"
#include "Serialization.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "ECS/SystemHandler.hpp"
#include "Resources/Scene.hpp"
#include "CGPMove.hpp"

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;
using namespace Cookie::ECS::System;
using namespace Cookie::Gameplay;
using namespace rp3d;

Editor::Editor()
    : ui(game.renderer) 
{
    game.resources.Load(game.renderer);
    game.skyBox.texture = game.resources.textures["Assets/skybox.dds"];
    game.renderer.AddFrameBuffer(game.resources);
    cam.SetProj(Core::Math::ToRadians(60.f), game.renderer.state.viewport.Width, game.renderer.state.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    cam.pos = { 0.f , 20.0f,30.0f };
    cam.rot = { Core::Math::ToRadians(30.0f) ,0.0f,0.0f };

    cam.ResetPreviousMousePos();
    cam.Update();
    cam.Deactivate();
    //scene = Editor::Scene(resources, coordinator);
    game.scene->InitCoordinator(game.coordinator);
    //game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"] = (std::make_shared<Resources::Texture>("Assets/Floor_DefaultMaterial_BaseColor.png"));

    //Load all prefabs in folder Prefabs
    Resources::Serialization::Load::LoadAllPrefabs(game.resources);

    //Load all Textures we have create in texture editor
    Resources::Serialization::Load::LoadAllTextures(game.resources);

    //Load default Scene
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/DefaultDuck.CAsset", game);

    game.SetScene(_scene);

    ui.AddItem(new UIwidget::SaveButton(game.scene, game.resources), 0);

    ui.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    ui.AddWItem(new UIwidget::TextureEditor(game.renderer, game.resources), 1);

    ui.AddWItem(new UIwidget::Inspector(selectedEntity, game.resources, game.coordinator), 2);

    ui.AddWItem(new UIwidget::FileExplorer(game.renderer, game), 2);

    ui.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);

    ui.AddWItem(new UIwidget::Console(CDebug, game.renderer), 2);

    ui.AddWItem(new UIwidget::DemoWindow, 3);


    UIwidget::Toolbar* toolbar = new UIwidget::Toolbar(game.renderer);
    ui.AddWindow(new UIwidget::Viewport(toolbar, game.renderer.window.window, game.renderer.GetLastFrameBuffer(), &cam, game.coordinator, selectedEntity));

    InitEditComp();

    Physics::PhysicsHandle::editWorld->setIsDebugRenderingEnabled(false);
}

Editor::~Editor()
{
    //Save all prefabs in folder Prefabs
    Resources::Serialization::Save::SaveAllPrefabs(game.resources);
}


void Editor::InitEditComp()
{
    for (int i = 1; i < MAX_ENTITIES; i++)
    {
        editingComponent[i].InitComponent(game.coordinator.componentHandler->GetComponentTransform(i));
    }
}

void Editor::ModifyEditComp()
{
    for (int i = 1; i < MAX_ENTITIES; i++)
    {
        editingComponent[i].editTrs = &game.coordinator.componentHandler->GetComponentTransform(i);
        if ((game.coordinator.entityHandler->entities[i].signature & SIGNATURE_MODEL) && game.coordinator.componentHandler->GetComponentModel(i).mesh != nullptr)
        {
            editingComponent[i].AABBMin = game.coordinator.componentHandler->GetComponentModel(i).mesh->AABBMin;
            editingComponent[i].AABBMax = game.coordinator.componentHandler->GetComponentModel(i).mesh->AABBMax;
            editingComponent[i].MakeCollider();
        }
        editingComponent[i].Update();
    }
}


void Editor::Loop()
{
    //soundManager.system->playSound(soundManager.sound, nullptr, false, nullptr);
    Physics::PhysicsHandle physHandle;

    Vec2 mousePos;
    {
        game.scene->map.model.mesh                 = game.resources.meshes["Cube"];
        game.scene->map.model.texture              = game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"];
        game.scene->map.model.shader               = game.resources.shaders["Texture_Shader"];


        //will be removed after testing phase
        game.scene->map.modelTileObstacle.mesh     = game.resources.meshes["Cube"];
        game.scene->map.modelTileObstacle.texture  = game.resources.textures["Grey"];
        game.scene->map.modelTileObstacle.shader   = game.resources.shaders["Texture_Shader"];
    }
    ComponentTransform buildingTrs;
    ComponentModel     buildingModel;
    Vec2 buildingTileSize {{1, 1}};
    int nbOfBuildings = 0;
    int nbOfUnits = 0;
    bool isRaycastingWithMap = false;
    int indexOfSelectedTile = 0;
    {
        buildingTrs.scale.x = buildingTileSize.x * game.scene->map.tilesSize.x;
        buildingTrs.scale.z = buildingTileSize.y * game.scene->map.tilesSize.y;
        buildingModel.mesh = game.resources.meshes["Cube"];
        buildingModel.texture = game.resources.textures["Pink"];
        buildingModel.shader = game.resources.shaders["Texture_Shader"];
    }
    Vec2 selectionQuadStart;
    bool makingASelectionQuad = false;

    while (!glfwWindowShouldClose(game.renderer.window.window))
    {
        // Present frame
        CDebug.UpdateTime();
        
        game.resources.UpdateScriptsContent();
        game.coordinator.ApplyScriptUpdate();

        // Present frame
        if (isPlaying)
        {
            game.Update();
        }
        else
        {
            glfwPollEvents();
            game.TryResizeWindow();
            game.renderer.Clear();

            cam.Update();
        }

        if (currentScene != game.scene.get())
        {
            selectedEntity = {};
            selectedEntity.componentHandler = game.coordinator.componentHandler;
            ModifyEditComp();
            currentScene = game.scene.get();
        }

        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
            Resources::Serialization::Save::SaveScene(*game.scene, game.resources);

        ////TEMP
        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_H) == GLFW_PRESS)
        {
            std::string duck = "Duck";
            game.coordinator.componentHandler->ModifyComponentOfEntityToPrefab(game.coordinator.entityHandler->entities[1], game.resources, duck);
        }
        if (!ImGui::GetIO().MouseDownDuration[0])
        {            
            Core::Math::Vec3 fwdRay = cam.pos + cam.MouseToWorldDir() * cam.camFar;
            rp3d::Ray ray({ cam.pos.x,cam.pos.y,cam.pos.z }, {fwdRay.x,fwdRay.y,fwdRay.z});
            physHandle.editWorld->raycast(ray,this);
        }

        
        //Raycast with Map
        if(isRaycastingWithMap)
        {
            Core::Math::Vec3 fwdRay = cam.pos + cam.MouseToWorldDir() * cam.camFar;
            rp3d::Ray ray({ cam.pos.x,cam.pos.y,cam.pos.z }, { fwdRay.x,fwdRay.y,fwdRay.z });  
            RaycastInfo raycastInfo;

            //if raycast hit
            if (game.scene->map.physic.physBody->raycast(ray, raycastInfo))
            {
                Vec3 hitPoint{ raycastInfo.worldPoint.x, raycastInfo.worldPoint.y, raycastInfo.worldPoint.z };
                //hitPoint.Debug();

                mousePos =  {{hitPoint.x, hitPoint.z}};
                indexOfSelectedTile = game.scene->map.GetTileIndex(mousePos);
                Vec2 centerOfBuilding = game.scene->map.GetCenterOfBuilding(mousePos, buildingTileSize);

                buildingTrs.pos = {centerOfBuilding.x, hitPoint.y, centerOfBuilding.y};
            }
        }
        //Bind Keys to change Nb of Tiles of Building
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_K])
            {
                buildingTileSize.x = std::fmax(1, buildingTileSize.x - 1);
                buildingTrs.scale.x = buildingTileSize.x * game.scene->map.tilesSize.x;
            }
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_L])
            {
                buildingTileSize.y = std::fmax(1, buildingTileSize.y - 1);
                buildingTrs.scale.z = buildingTileSize.y * game.scene->map.tilesSize.y;
            }
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_I])
            {
                buildingTileSize.x = std::fmin(game.scene->map.tilesNb.x, buildingTileSize.x + 1);
                buildingTrs.scale.x = buildingTileSize.x * game.scene->map.tilesSize.x;
            }
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_O])
            {
                buildingTileSize.y = std::fmin(game.scene->map.tilesNb.y, buildingTileSize.y + 1);
                buildingTrs.scale.z = buildingTileSize.y * game.scene->map.tilesSize.y;
            }
        }
        //Bind Keys to activate/deactivate raycast with map 
        {
            //Key M on Azerty Keyboard
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_SEMICOLON])
            {
                isRaycastingWithMap = !isRaycastingWithMap;
            }
        }
        //Bind Keys to create Building
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_P] && isRaycastingWithMap)
            {
                game.coordinator.AddEntity(SIGNATURE_TRANSFORM + SIGNATURE_MODEL, game.resources, "Building " + std::to_string(nbOfBuildings) );
                //should create constructor copy for each Component 
                {
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(game.coordinator.entityHandler->livingEntities - 1);
                    ComponentModel& model   = game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->livingEntities - 1);

                    trs.pos = buildingTrs.pos;
                    trs.scale = buildingTrs.scale;

                    model.mesh = buildingModel.mesh;
                    model.texture = game.resources.textures["Green"];
                    model.shader = buildingModel.shader;
                }

                nbOfBuildings++;
            }
        }
        //Bind Keys to Set Obstacle Tiles
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_B] && isRaycastingWithMap)
            {
                game.scene->map.tiles[indexOfSelectedTile].isObstacle = !game.scene->map.tiles[indexOfSelectedTile].isObstacle;
            }
        }
        //Bind Keys to give orders to Units
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_G] && isRaycastingWithMap && game.coordinator.selectedEntities.size() != 0)
            {
                ECS::Entity* commander = game.coordinator.GetSelectedEntitiesCommander();
                game.coordinator.SetSelectedEntitiesCommander(commander);

                ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(commander->id);
                if (game.scene->map.ApplyPathfinding(game.scene->map.GetTile(trs.pos), game.scene->map.tiles[indexOfSelectedTile]))
                    game.coordinator.componentHandler->GetComponentGameplay(commander->id).componentMove.SetPath(game.scene->map.tiles[indexOfSelectedTile], trs);
                else
                    std::cout << "No Path Find\n";

            }
        }
        //Bind Key to create new unit
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_N] && isRaycastingWithMap)
            {
                game.coordinator.AddEntity(SIGNATURE_TRANSFORM + SIGNATURE_MODEL + SIGNATURE_GAMEPLAY, game.resources, "Unit " + std::to_string(nbOfUnits));
                //should create constructor copy for each Component 
                {
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(game.coordinator.entityHandler->livingEntities - 1);
                    ComponentModel& model = game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->livingEntities - 1);
                    game.coordinator.entityHandler->entities[game.coordinator.entityHandler->livingEntities - 1].signatureGameplay = SIGNATURE_CGP_ALL;

                    trs.pos = {mousePos.x, 0, mousePos.y};

                    model.mesh = game.resources.meshes["Cube"];
                    model.texture = game.resources.textures["Green"];
                    model.shader = game.resources.shaders["Texture_Shader"];

                }

                nbOfUnits++;
            }
        }
        //Selection Quad
        {
            if (ImGui::GetIO().MouseClicked[0] && isRaycastingWithMap)
            {
                makingASelectionQuad = true;
                selectionQuadStart = mousePos;
            }
            if (makingASelectionQuad)
            {
                dbgRenderer.AddDebugElement(Core::Primitives::CreateLine({ selectionQuadStart.x, 1, selectionQuadStart.y }, { mousePos.x, 1, selectionQuadStart.y }, 0x00FF00, 0x00FF00));
                dbgRenderer.AddDebugElement(Core::Primitives::CreateLine({ selectionQuadStart.x, 1, selectionQuadStart.y }, { selectionQuadStart.x, 1, mousePos.y }, 0x00FF00, 0x00FF00));
                dbgRenderer.AddDebugElement(Core::Primitives::CreateLine({ mousePos.x, 1, selectionQuadStart.y }, { mousePos.x, 1, mousePos.y }, 0x00FF00, 0x00FF00));
                dbgRenderer.AddDebugElement(Core::Primitives::CreateLine({ selectionQuadStart.x, 1, mousePos.y }, { mousePos.x, 1, mousePos.y }, 0x00FF00, 0x00FF00));
            }
            if (ImGui::GetIO().MouseReleased[0] && isRaycastingWithMap)
            {
                makingASelectionQuad = false;
                game.coordinator.SelectEntities(selectionQuadStart, mousePos);
            }
        }

        if (selectedEntity.toChangeEntityId >= 0)
        {
            PopulateFocusedEntity();
        }
        if (selectedEntity.focusedEntity && (selectedEntity.focusedEntity->signature & SIGNATURE_PHYSICS))
        {
            selectedEntity.componentHandler->componentPhysics[selectedEntity.focusedEntity->id].Set(selectedEntity.componentHandler->componentTransforms[selectedEntity.focusedEntity->id]);
        }
           

        //game.scene->physSim.Update();
        //game.coordinator.ApplySystemPhysics(game.scene->physSim.factor);

        game.coordinator.ApplyGameplayUpdatePushedCooldown(game.scene->map);
        game.coordinator.ApplyGameplayMoveTowardWaypoint();
        game.coordinator.ApplyGameplayMoveWithCommander();
        game.coordinator.ApplyGameplayPosPrediction();
        game.coordinator.ApplyGameplayResolveCollision();
        game.coordinator.ApplyGameplayDrawPath(dbgRenderer);
        

        game.renderer.Draw(&cam, game);
        if (isRaycastingWithMap)
            SystemDraw(buildingTrs, buildingModel, cam.GetViewProj());
        game.scene->map.DrawSpecificTiles(cam.GetViewProj());

        dbgRenderer.Draw(cam.GetViewProj());

        game.renderer.SetBackBuffer();

        ui.UpdateUI();

        game.renderer.Render();
    }
}
