#include "Editor.hpp" 
#include "UIallIn.hpp"
#include "Serialization.hpp"
#include "Primitives.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "ECS/SystemHandler.hpp"
#include "Resources/Scene.hpp"
#include "CGPMove.hpp"
#include "CGPProducer.hpp"

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;
using namespace Cookie::ECS::System;
using namespace Cookie::Gameplay;
using namespace rp3d;

Editor::Editor()
    : editorFBO{game.renderer.window.width,game.renderer.window.height}
{
    game.resources.Load(game.renderer);
    game.skyBox.texture = game.resources.textures["Assets/skybox.dds"].get();
    cam.SetProj(Core::Math::ToRadians(60.f), game.renderer.viewport.Width, game.renderer.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
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
    //std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/DefaultDuck.CAsset", game);
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/Default.CAsset", game);


    game.SetScene(_scene);

    editorUI.AddItem(new UIwidget::SaveButton(game.scene, game.resources), 0);
    editorUI.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    editorUI.AddWItem(new UIwidget::TextureEditor(game.resources), 1);
    editorUI.AddWItem(new UIwidget::GameUIeditor(game.renderer.window, game.scene), 1);

    editorUI.AddWItem(new UIwidget::GamePort(game), 2);
    editorUI.AddWItem(new UIwidget::Inspector(selectedEntity, game.resources, game.coordinator), 2);
    editorUI.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);
    editorUI.AddWItem(new UIwidget::WorldSettingsWidget(game.scene), 2);
    editorUI.AddWItem(new UIwidget::Console(CDebug, game.renderer), 2);
    editorUI.AddWItem(new UIwidget::FileExplorer(game.renderer, game), 2);

    editorUI.AddWItem(new UIwidget::DemoWindow, 3);


    UIwidget::Toolbar* toolbar = new UIwidget::Toolbar(game.renderer);
    editorUI.AddWindow(new UIwidget::Viewport(toolbar, game.renderer.window.window, editorFBO, &cam, game.coordinator, selectedEntity));

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
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        editingComponent[i].InitComponent(game.coordinator.componentHandler->GetComponentTransform(i));
    }
}

void Editor::ModifyEditComp()
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        editingComponent[i].editTrs = &game.coordinator.componentHandler->GetComponentTransform(i);
        if ((game.coordinator.entityHandler->entities[i].signature & C_SIGNATURE::MODEL) && game.coordinator.componentHandler->GetComponentModel(i).mesh != nullptr)
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
        game.scene->map.model.mesh                  = game.resources.meshes["NormalCube"].get();
        game.scene->map.model.albedo                = game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"].get();
    }
    Vec3 buildingPos;
    Vec2 buildingTileSize {{1, 1}};
    bool isBuildingValid = false;
    int nbOfBuildings = 0;
    int nbOfUnits = 0;
    bool isRaycastingWithMap = false;
    int indexOfSelectedTile = 0;
    bool makingASelectionQuad = false;
    Vec2 selectionQuadStart;

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
            TryResizeWindow();
            game.renderer.Clear();
            game.renderer.ClearFrameBuffer(editorFBO);

            cam.Update();
        }

        if (currentScene != game.scene.get())
        {
            selectedEntity = {};
            selectedEntity.componentHandler = game.coordinator.componentHandler;
            ModifyEditComp();
            currentScene = game.scene.get();
        }

        //if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
        //    Resources::Serialization::Save::SaveScene(*game.scene, game.resources);

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

                buildingPos = {centerOfBuilding.x, 1, centerOfBuilding.y};
            }
        }
        //Bind Keys to change Nb of Tiles of Building
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_K])
                buildingTileSize.x = std::fmax(1, buildingTileSize.x - 1);
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_L])
                buildingTileSize.y = std::fmax(1, buildingTileSize.y - 1);            
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_I])
                buildingTileSize.x = std::fmin(game.scene->map.tilesNb.x, buildingTileSize.x + 1);
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_O])
                buildingTileSize.y = std::fmin(game.scene->map.tilesNb.y, buildingTileSize.y + 1);
        }
        //Bind Keys to activate/deactivate raycast with map 
        {
            //Key M on Azerty Keyboard
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_SEMICOLON])
            {
                isRaycastingWithMap = !isRaycastingWithMap;
            }
        }
        //Bind Keys to check if building is valid and to create Building
        {
           
            if (isRaycastingWithMap)
            {
                Vec2 posTopLeft = {{buildingPos.x - buildingTileSize.x * game.scene->map.tilesSize.x / 2,
                                    buildingPos.z - buildingTileSize.y * game.scene->map.tilesSize.y / 2}};
                isBuildingValid = game.scene->map.isBuildingValid(game.scene->map.GetTileIndex(posTopLeft), buildingTileSize);
            }
            
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_P] && isRaycastingWithMap && isBuildingValid)
            {
                game.coordinator.AddEntity(C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL + C_SIGNATURE::GAMEPLAY, "Building " + std::to_string(nbOfBuildings) );
                //should create constructor copy for each Component 
                {
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(game.coordinator.entityHandler->livingEntities - 1);
                    ComponentModel& model   = game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->livingEntities - 1);
                    ComponentGameplay& gameplay = game.coordinator.componentHandler->GetComponentGameplay(game.coordinator.entityHandler->livingEntities - 1);
                    gameplay.AddComponent(CGP_SIGNATURE::PRODUCER);
                    CGPProducer& producer = gameplay.componentProducer;

                    trs.pos = buildingPos;
                    trs.scale = {buildingTileSize.x * game.scene->map.tilesSize.x, 1, buildingTileSize.y * game.scene->map.tilesSize.y};
                    trs.trsHasChanged = true;

                    model.mesh = game.resources.meshes["Cube"].get();
                    model.albedo = game.resources.textures["Blue"].get();

                    producer.tileSize = buildingTileSize;
                    Vec3 posTopLeft = trs.pos - trs.scale / 2;
                    game.scene->map.GiveTilesToBuilding(game.scene->map.GetTileIndex(posTopLeft), producer);
                }

                nbOfBuildings++;
            }
        }
        //Bind Keys to Set Obstacle Tiles
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_V] && isRaycastingWithMap)
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
                
                /*
                for (int i = 0; i < game.coordinator.selectedEntities.size(); ++i)
                {
                    float selectedEntityId = game.coordinator.selectedEntities[i]->id;
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(selectedEntityId);
                    if (game.scene->map.ApplyPathfinding(game.scene->map.GetTile(trs.pos), game.scene->map.tiles[indexOfSelectedTile]))
                        game.coordinator.componentHandler->GetComponentGameplay(selectedEntityId).componentMove.SetPath(game.scene->map.tiles[indexOfSelectedTile], trs);
                }
                */
            }
        }
        //Bind Key to create new unit
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_N] && isRaycastingWithMap)
            {
                game.coordinator.AddEntity(C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL + C_SIGNATURE::GAMEPLAY, "Unit " + std::to_string(nbOfUnits));
                //should create constructor copy for each Component 
                {
                    ECS::Entity& entity = game.coordinator.entityHandler->entities[game.coordinator.entityHandler->livingEntities - 1];
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(entity.id);
                    ComponentModel& model = game.coordinator.componentHandler->GetComponentModel(entity.id);
                    ComponentGameplay& gameplay = game.coordinator.componentHandler->GetComponentGameplay(entity.id);
                    entity.tag = "good";
                    gameplay.signatureGameplay = CGP_SIGNATURE::ALL_CGP;

                    trs.pos = { mousePos.x, 1, mousePos.y };
                    trs.trsHasChanged = true;

                    model.mesh = game.resources.meshes["Cube"].get();
                    model.albedo = game.resources.textures["Green"].get();
                    //model.shader = game.resources.shaders["Texture_Shader"];

                }

                nbOfUnits++;
            }
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_B] && isRaycastingWithMap)
            {
                game.coordinator.AddEntity(C_SIGNATURE::TRANSFORM + C_SIGNATURE::MODEL + C_SIGNATURE::GAMEPLAY, "Unit " + std::to_string(nbOfUnits));
                //should create constructor copy for each Component 
                {
                    ECS::Entity& entity = game.coordinator.entityHandler->entities[game.coordinator.entityHandler->livingEntities - 1];
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(entity.id);
                    ComponentModel& model = game.coordinator.componentHandler->GetComponentModel(entity.id);
                    ComponentGameplay& gameplay = game.coordinator.componentHandler->GetComponentGameplay(entity.id);
                    entity.tag = "bad";
                    gameplay.signatureGameplay = CGP_SIGNATURE::ALL_CGP;

                    trs.pos = { mousePos.x, 1, mousePos.y };
                    trs.trsHasChanged = true;

                    model.mesh = game.resources.meshes["Cube"].get();
                    model.albedo = game.resources.textures["Red"].get();
                    //model.shader = game.resources.shaders["Texture_Shader"];

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
                //use 1 for Y so the debug will not be mix up with the map
                dbgRenderer.AddQuad({ selectionQuadStart.x, 1, selectionQuadStart.y }, { mousePos.x, 1, mousePos.y }, 0x00FF00);
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
        if (selectedEntity.focusedEntity && (selectedEntity.focusedEntity->signature & C_SIGNATURE::PHYSICS))
        {
            selectedEntity.componentHandler->GetComponentPhysics(selectedEntity.focusedEntity->id).Set(selectedEntity.componentHandler->GetComponentTransform(selectedEntity.focusedEntity->id));
        }
           

        //game.scene->physSim.Update();
        //game.coordinator.ApplySystemPhysics(game.scene->physSim.factor);

        game.coordinator.ApplyGameplayUpdatePushedCooldown(game.scene->map);
        game.coordinator.ApplyGameplayMoveTowardWaypoint();
        game.coordinator.ApplyGameplayMoveWithCommander();
        game.coordinator.ApplyGameplayPosPrediction();
        game.coordinator.ApplyGameplayResolveCollision();

        game.coordinator.ApplyGameplayCheckEnemyInRange();
        game.coordinator.ApplyGameplayAttack();


        game.coordinator.ApplyComputeTrs();
        game.coordinator.ApplyGameplayDrawPath(dbgRenderer);
        game.renderer.Draw(&cam, game,editorFBO);
        if (isRaycastingWithMap)
            dbgRenderer.AddQuad(buildingPos, buildingTileSize.x * game.scene->map.tilesSize.x / 2, buildingTileSize.y * game.scene->map.tilesSize.y / 2, (isBuildingValid) ? 0x00FF00 : 0xFF0000);
        dbgRenderer.Draw(cam.GetViewProj());

        game.renderer.SetBackBuffer();

        UIcore::BeginFrame();
        editorUI.UpdateUI();
        UIcore::EndFrame();

        game.renderer.Render();

        game.coordinator.ApplyRemoveUnnecessaryEntities();
    }
}

void Editor::TryResizeWindow()
{
    int width = 0;
    int height = 0;

    glfwGetWindowSize(game.renderer.window.window, &width, &height);

    if (width <= 0 || height <= 0)
        return;

    if (game.renderer.window.width != width || game.renderer.window.height != height)
    {
        //Core::DebugMessageHandler::Summon().Log((std::to_string(width) + ' ' + std::to_string(height)).c_str());
        //printf("%d, %d\n", width, height);
        game.renderer.window.width = width;
        game.renderer.window.height = height;

        game.renderer.ResizeBuffer(width, height);
        game.frameBuffer.Resize(width, height);
        editorFBO.Resize(width, height);

        //scene->camera->SetProj(Core::Math::ToRadians(60.f), width, height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}
