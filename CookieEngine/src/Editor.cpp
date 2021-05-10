#include "Editor.hpp" 
#include "UIallIn.hpp"
#include "Serialization.hpp"
#include "Primitives.hpp"
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
    : ui(game.renderer),
    editorFBO{game.renderer.window.width,game.renderer.window.height}
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
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/DefaultDuck.CAsset", game);

    game.SetScene(_scene);

    ui.AddItem(new UIwidget::SaveButton(game.scene, game.resources), 0);

    ui.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    ui.AddWItem(new UIwidget::TextureEditor(game.resources), 1);

    ui.AddWItem(new UIwidget::Inspector(selectedEntity, game.resources, game.coordinator), 2);

    ui.AddWItem(new UIwidget::FileExplorer(game.renderer, game), 2);

    ui.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);

    ui.AddWItem(new UIwidget::Console(CDebug, game.renderer), 2);

    ui.AddWItem(new UIwidget::DemoWindow, 3);


    UIwidget::Toolbar* toolbar = new UIwidget::Toolbar(game.renderer);
    ui.AddWindow(new UIwidget::Viewport(toolbar, game.renderer.window.window, editorFBO, &cam, game.coordinator, selectedEntity));

    InitEditComp();

    Physics::PhysicsHandle::editWorld->setIsDebugRenderingEnabled(false);
}

Editor::~Editor()
{
    ui.Terminate();
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
        game.scene->map.model.mesh                 = game.resources.meshes["Cube"].get();
        game.scene->map.model.albedo              = game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"].get();


        //will be removed after testing phase
        game.scene->map.modelTileStart.mesh        = game.resources.meshes["Cube"].get();
        game.scene->map.modelTileStart.albedo     = game.resources.textures["Green"].get();

        game.scene->map.modelTileEnd.mesh          = game.resources.meshes["Cube"].get();
        game.scene->map.modelTileEnd.albedo       = game.resources.textures["Red"].get();

        game.scene->map.modelTileObstacle.mesh     = game.resources.meshes["Cube"].get();
        game.scene->map.modelTileObstacle.albedo  = game.resources.textures["Grey"].get();
    }
    ComponentTransform buildingTrs;
    ComponentModel     buildingModel;
    Vec2 buildingTileSize {{1, 1}};
    int nbOfBuildings = 0;
    bool isRaycastingWithMap = false;
    int indexOfSelectedTile = 0;
    {
        buildingTrs.scale.x = buildingTileSize.x * game.scene->map.tilesSize.x;
        buildingTrs.scale.z = buildingTileSize.y * game.scene->map.tilesSize.y;
        buildingModel.mesh = game.resources.meshes["Cube"].get();
        buildingModel.albedo = game.resources.textures["Pink"].get();
    }

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
                hitPoint.Debug();

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
        //Bind Keys to create Building
        {
            //Key M on Azerty Keyboard
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_SEMICOLON])
            {
                isRaycastingWithMap = !isRaycastingWithMap;
            }
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
                    model.albedo = game.resources.textures["Green"].get();
                }

                nbOfBuildings++;
            }
        }
        //Bind Keys to Set Obstacle Tiles and to give orders to Units
        {
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_B] && isRaycastingWithMap)
            {
                game.scene->map.tiles[indexOfSelectedTile].isObstacle = !game.scene->map.tiles[indexOfSelectedTile].isObstacle;
            }
            if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_G] && isRaycastingWithMap)
            {
                float selectedEntityId = selectedEntity.focusedEntity->id;
                ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(selectedEntityId);
                Vec2 PosOnMap = { {trs.pos.x, trs.pos.z} };
                game.scene->map.tileStart = &game.scene->map.tiles[game.scene->map.GetTileIndex(PosOnMap)];
                game.scene->map.tileEnd = &game.scene->map.tiles[indexOfSelectedTile];
                game.scene->map.ApplyPathfinding();
                game.coordinator.componentHandler->GetComponentGameplay(selectedEntityId).componentMove.SetPath(*game.scene->map.tileEnd);
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
        game.coordinator.ApplyGameplayMove();

        game.renderer.Draw(&cam, game,editorFBO);

        game.scene->map.DrawPath(dbgRenderer);

        dbgRenderer.Draw(cam.GetViewProj());

        game.renderer.SetBackBuffer();
        ui.UpdateUI();

        game.renderer.Render();
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
