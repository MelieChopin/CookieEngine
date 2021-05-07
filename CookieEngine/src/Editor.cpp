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
    : editorFBO{game.renderer.window.width,game.renderer.window.height}
{
    game.resources.Load(game.renderer);
    game.skyBox.texture = game.resources.textures["Assets/skybox.dds"];
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
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/Default.CAsset", game);

    game.SetScene(_scene);

    editorUI.AddItem(new UIwidget::SaveButton(game.scene, game.resources), 0);
    editorUI.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    editorUI.AddWItem(new UIwidget::TextureEditor(game.resources), 1);
    editorUI.AddWItem(new UIwidget::GameUIeditor(game.renderer.window, game.scene), 1);

    editorUI.AddWItem(new UIwidget::GamePort(game), 2);
    editorUI.AddWItem(new UIwidget::Inspector(selectedEntity, game.resources, game.coordinator), 2);
    editorUI.AddWItem(new UIwidget::FileExplorer(game.renderer, game), 2);
    editorUI.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);
    editorUI.AddWItem(new UIwidget::Console(CDebug, game.renderer), 2);

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


        //will be removed after testing phase
        game.scene->map.modelTileObstacle.mesh     = game.resources.meshes["Cube"];
        game.scene->map.modelTileObstacle.texture  = game.resources.textures["Grey"];
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
    }
    Vec2 selectionQuadStart;
    bool makingASelectionQuad = false;

    /// Particles
    game.particlesSystem = Cookie::Resources::Particles::ParticlesSystem(40, 20);
    game.particlesSystem.shader.InitShader();
    game.particlesSystem.trs.pos = Vec3(0, 10, 0);
    game.particlesSystem.trs.rot = Vec3(0, 0, 0);
    game.particlesSystem.trs.scale = Vec3(1, 1, 1);
    game.particlesSystem.trs.ComputeTRS();

    Cookie::Resources::Particles::BoxPositionGenerate       box(Vec3(0, 0, 0), Vec3(2, 2, 2));
    Cookie::Resources::Particles::CirclePositionGenerate    circle(Vec3(0, 0, 0), 2.0f);
    Cookie::Resources::Particles::VelocityConstGenerate     vel(Vec3(0, 10, 0));
    Cookie::Resources::Particles::VelocityRandGenerate      velRand(Vec3(0, 15, 0), Vec3(0, 35, 0));
    Cookie::Resources::Particles::MassConstGenerate         mass(2);
    Cookie::Resources::Particles::TimeGenerate              time(2);
    Cookie::Resources::Particles::TimeRandGenerate          timeRand(0.25f, 0.55f);
    Cookie::Resources::Particles::ColorRandGenerate         color(Vec3(0.5f, 0, 0), Vec3(1, 0, 0));
    game.particlesSystem.data[0].countAlive = 10;
    game.particlesSystem.particlesEmiter[0].generators.push_back(&circle);
    game.particlesSystem.particlesEmiter[0].generators.push_back(&velRand);
    //particlesSystem.particlesEmiter[0].generators.push_back(&mass);
    game.particlesSystem.particlesEmiter[0].generators.push_back(&timeRand);
    game.particlesSystem.particlesEmiter[0].generators.push_back(&color);
    for (int i = 0; i < game.particlesSystem.particlesEmiter[0].generators.size(); i++)
        game.particlesSystem.particlesEmiter[0].generators[i]->generate(&game.particlesSystem.data[0], 0, 10);

    /*game.particlesSystem.data.push_back(Cookie::Resources::Particles::ParticlesData());
    game.particlesSystem.particlesEmiter.push_back(Cookie::Resources::Particles::ParticlesEmitter());
    game.particlesSystem.data[1].generate(10, 3);
    game.particlesSystem.particlesEmiter[1].generators.push_back(&box);
    game.particlesSystem.particlesEmiter[1].generators.push_back(&vel);
    game.particlesSystem.particlesEmiter[1].generators.push_back(&mass);
    game.particlesSystem.particlesEmiter[1].generators.push_back(&timeRand);
    for (int i = 0; i < game.particlesSystem.particlesEmiter[1].generators.size(); i++)
        game.particlesSystem.particlesEmiter[1].generators[i]->generate(&game.particlesSystem.data[1], 0, 5);*/



    Cookie::Resources::Particles::UpdateVelocity    updateVel;
    Cookie::Resources::Particles::UpdateScale       updateScale;
    Cookie::Resources::Particles::EnabledGravity    enabledGravity;
    Cookie::Resources::Particles::UpdateTime        updateTime;
   // particlesSystem.particlesEmiter.updates.push_back(&enabledGravity);
    game.particlesSystem.particlesEmiter[0].updates.push_back(&updateVel);
    game.particlesSystem.particlesEmiter[0].updates.push_back(&updateTime);
    game.particlesSystem.particlesEmiter[0].updates.push_back(&updateScale);

   /* game.particlesSystem.particlesEmiter[1].updates.push_back(&updateVel);
    game.particlesSystem.particlesEmiter[1].updates.push_back(&updateTime);
    game.particlesSystem.particlesEmiter[1].updates.push_back(&enabledGravity);*/

    std::vector<Cookie::Resources::Particles::ParticlesSystem> partsys;
    for (int i = 0; i < 4; i++)
        partsys.push_back(game.particlesSystem);
    partsys[0].trs.TRS = Cookie::Core::Math::Mat4::Translate( Vec3(-10, 10, -10));
    partsys[1].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(-10, 10, 10));
    partsys[2].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(10, 10, -10));
    partsys[3].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(10, 10, 10));
    ///


    bool isActive = false;

    while (!glfwWindowShouldClose(game.renderer.window.window))
    {
        if (isActive)
        {
            game.particlesSystem.Update();
            for (int i = 0; i < 4; i++)
                partsys[i].Update();
        }
            
            
        
        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
            isActive = true;

        
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

       // if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
        //    Resources::Serialization::Save::SaveScene(*game.scene, game.resources);

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
                game.coordinator.AddEntity(SIGNATURE_TRANSFORM + SIGNATURE_MODEL + SIGNATURE_GAMEPLAY, game.resources, "Unit " + std::to_string(nbOfUnits));
                //should create constructor copy for each Component 
                {
                    ComponentTransform& trs = game.coordinator.componentHandler->GetComponentTransform(game.coordinator.entityHandler->livingEntities - 1);
                    ComponentModel& model = game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->livingEntities - 1);
                    game.coordinator.entityHandler->entities[game.coordinator.entityHandler->livingEntities - 1].signatureGameplay = SIGNATURE_CGP_ALL;

                    trs.pos = {mousePos.x, 1, mousePos.y};

                    model.mesh = game.resources.meshes["Cube"];
                    model.texture = game.resources.textures["Green"];
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
        

        game.renderer.Draw(&cam, game,editorFBO);
        //if (isRaycastingWithMap)
            //SystemDraw(buildingTrs, buildingModel, cam.GetViewProj());
        //game.scene->map.DrawSpecificTiles(cam.GetViewProj());
        game.particlesSystem.Draw(cam.GetProj(), cam.GetView(), game.resources);
        for (int i = 0; i < 4; i++)
            partsys[i].Draw(cam.GetProj(), cam.GetView(), game.resources);
        dbgRenderer.Draw(cam.GetViewProj());

        game.renderer.SetBackBuffer();

        UIcore::BeginFrame();
        editorUI.UpdateUI();
        UIcore::EndFrame();

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
        Core::DebugMessageHandler::Summon().Log((std::to_string(width) + ' ' + std::to_string(height)).c_str());
        printf("%d, %d\n", width, height);
        game.renderer.window.width = width;
        game.renderer.window.height = height;

        game.renderer.ResizeBuffer(width, height);
        game.frameBuffer.Resize(width, height);
        editorFBO.Resize(width, height);

        //scene->camera->SetProj(Core::Math::ToRadians(60.f), width, height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}
