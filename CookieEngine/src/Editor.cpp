#include "Editor.hpp" 
#include "UIcore.hpp"
#include "UIeditor_AllIn.hpp"
#include "Serialization.hpp"
#include "Primitives.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Resources/Scene.hpp"
#include "CGPMove.hpp"
#include "CGPProducer.hpp"
#include "SoundManager.hpp"

using namespace Cookie;
using namespace Cookie::Core;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;
using namespace Cookie::Gameplay;
using namespace Cookie::Resources;
using namespace rp3d;

Editor::Editor()
    : editorFBO{game.renderer.window.width,game.renderer.window.height}
{
    game.resources.Load(game.renderer);
    game.skyBox.texture = game.resources.skyboxes["Assets/skybox.dds"].get();
    cam.SetProj(60.f, game.renderer.viewport.Width, game.renderer.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    cam.pos = { 0.f , 20.0f,30.0f };
    cam.rot = { Core::Math::ToRadians(30.0f) ,0.0f,0.0f };

    cam.ResetPreviousMousePos();
    cam.Update();
    cam.Deactivate();
    //scene = Editor::Scene(resources, coordinator);
    game.scene->InitCoordinator(game.coordinator);
    //game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"] = (std::make_shared<Resources::Texture>("Assets/Floor_DefaultMaterial_BaseColor.png"));

    //Load all Textures we have create in texture editor
    Resources::Serialization::Load::LoadAllTextures(game.resources);

    //Load all prefabs in folder Prefabs
    Resources::Serialization::Load::LoadAllPrefabs(game.resources);

    Resources::SoundManager::InitSystem();
    Resources::SoundManager::LoadAllMusic(game.resources);

    //Load default Scene
    //std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/DefaultDuck.CAsset", game);
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/Default.CAsset", game);


    game.SetScene(_scene);

    editorUI.AddItem(new UIwidget::SaveButton(game.scene, game.resources), 0);
    editorUI.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);
    
    editorUI.AddWItem(new UIwidget::TextureEditor(game.resources), 1);
    editorUI.AddWItem(new UIwidget::GameUIeditor(game.renderer.window, game.scene), 1);
    editorUI.AddWItem(new UIwidget::SoundOrchestrator(), 1);
    
    editorUI.AddWItem(new UIwidget::GamePort(isPlaying, game), 2);
    editorUI.AddWItem(new UIwidget::Inspector(selectedEntity, game.resources, game.coordinator), 2);
    editorUI.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);
    editorUI.AddWItem(new UIwidget::WorldSettingsWidget(game.scene, game.renderer.lights, game.skyBox, game.resources), 2);
    editorUI.AddWItem(new UIwidget::Console(CDebug, game.resources), 2);
    editorUI.AddWItem(new UIwidget::FileExplorer(game), 2);

    editorUI.AddWItem(new UIwidget::DemoWindow, 3);


    UIwidget::Toolbar* toolbar = new UIwidget::Toolbar(game.resources, isPlaying);
    editorUI.AddWindow(new UIwidget::Viewport(toolbar, game.renderer.window.window, editorFBO, &cam, game.coordinator, selectedEntity));

    InitEditComp();

    Physics::PhysicsHandle::editWorld->setIsDebugRenderingEnabled(false);
}

Editor::~Editor()
{
    //Save all prefabs in folder Prefabs
   // Resources::Serialization::Save::SaveAllPrefabs(game.resources);
    Resources::SoundManager::Release();
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
    //Cookie::Resources::SoundManager::PlayMusic("Music.mp3");
    Physics::PhysicsHandle physHandle;

    /// Particles
    
        //First Particles 
        Cookie::Resources::Particles::ParticlesSystem first = Cookie::Resources::Particles::ParticlesSystem(2, 1);
        first.trs.pos = Vec3(0, 3, 0);
        first.trs.rot = Vec3(0, 0, 0);
        first.trs.scale = Vec3(1, 1, 1);
        first.trs.ComputeTRS();

        Cookie::Resources::Particles::BoxPositionGenerate       box(Vec3(0, 2, 0), Vec3(0.5, 0.5, 0.5), &first.trs.TRS);
        Cookie::Resources::Particles::VelocityConstGenerate     vel(Vec3(0, 10, 0));
        Cookie::Resources::Particles::VelocityRandGenerate      velRand(Vec3(-1, -1, -1), Vec3(2, 3, 2));
        Cookie::Resources::Particles::MassConstGenerate         mass(1);
        Cookie::Resources::Particles::TimeConstGenerate         time(2);
        Cookie::Resources::Particles::AccelerationRandGenerate  acc(Vec3(-10, -10, -10), Vec3(10, 10, 10));
        Cookie::Resources::Particles::TimeRandGenerate          timeRand(1.0f, 2.25f);
        Cookie::Resources::Particles::ColorConstGenerate        color(Vec4(1, 1, 1, 1));
        Cookie::Resources::Particles::RotateRandGenerate        rot(Vec3(Math::PI/2, 0, 0), Vec3(Math::PI / 2, 0, Math::PI));
        first.data[0].countAlive = 1;
        first.data[0].SetIsBIllboard(false);
        first.data[0].mesh = game.resources.meshes["Quad"].get();
        first.data[0].texture = game.resources.textures2D["Assets/VFX/Flaque.png"].get();
        //first.particlesEmiter[0].generators.push_back(&velRand);
        first.particlesEmiter[0].generators.push_back(&timeRand);
        first.particlesEmiter[0].generators.push_back(&rot);
        //first.particlesEmiter[0].generators.push_back(&mass);
        //first.particlesEmiter[0].generators.push_back(&acc);
        //for (int i = 0; i < first.particlesEmiter[0].generators.size(); i++)
        //    first.particlesEmiter[0].generators[i]->generate(&first.data[0], 0, first.data[0].countAlive);

        Cookie::Resources::Particles::UpdateVelocity    updateVel;
        Cookie::Resources::Particles::UpdateScale       updateScale(Math::Vec3(4, 4, 4));
        Cookie::Resources::Particles::EnabledGravity    enabledGravity(-9.81f);
        Cookie::Resources::Particles::UpdateTime        updateTime;
        Cookie::Resources::Particles::UpdateAlpha       alpha;
        Cookie::Resources::Particles::UpdateAcc         accu;
        Cookie::Resources::Particles::SlowDown          slow(0.45f);
        Cookie::Resources::Particles::ColorOverLife     col(Vec4(1.5f, 1, 0, 0));
        Cookie::Resources::Particles::CollisionWithPlane collisionWithPlane(Cookie::Core::Math::Vec3(0, 1, 0), -1);
        //first.particlesEmiter[0].updates.push_back(&updateVel);
        first.particlesEmiter[0].updates.push_back(&updateTime);
        first.particlesEmiter[0].updates.push_back(&alpha);
        //first.particlesEmiter[0].updates.push_back(&accu);
        //first.particlesEmiter[0].updates.push_back(&slow);
        first.particlesEmiter[0].updates.push_back(&updateScale);
        //first.particlesEmiter[0].updates.push_back(&collisionWithPlane);
        //first.particlesEmiter[0].updates.push_back(&enabledGravity);

        //for (int i = 0; i < 5; i++) // Create 5
            game.particlesHandler.particlesSystems.push_back(first);

        game.particlesHandler.particlesSystems[0].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(10, 1, 0));
        Cookie::Resources::Particles::PointPositionGenerate circle(Vec3(0, 0, 0), &game.particlesHandler.particlesSystems[0].trs.TRS);
        game.particlesHandler.particlesSystems[0].particlesEmiter[0].generators.push_back(&circle);
        Cookie::Resources::Particles::Loop loop(game.particlesHandler.particlesSystems[0].particlesEmiter[0].generators);
        //game.particlesHandler.particlesSystems[0].particlesEmiter[0].updates.push_back(&loop);

        /*game.particlesHandler.particlesSystems[1].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(-10, 5, 10));
        Cookie::Resources::Particles::SpherePositionGenerate circle2(Vec3(0, 0, 0), 1, &game.particlesHandler.particlesSystems[1].trs.TRS);
        game.particlesHandler.particlesSystems[1].particlesEmiter[0].generators.push_back(&circle2);
        Cookie::Resources::Particles::Loop loop1(game.particlesHandler.particlesSystems[1].particlesEmiter[0].generators);
        //game.particlesHandler.particlesSystems[1].particlesEmiter[0].updates.push_back(&loop1);

        game.particlesHandler.particlesSystems[2].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(10, 5, -10));
        Cookie::Resources::Particles::SpherePositionGenerate circle3(Vec3(0, 0, 0), 1, &game.particlesHandler.particlesSystems[2].trs.TRS);
        game.particlesHandler.particlesSystems[2].particlesEmiter[0].generators.push_back(&circle3);
        Cookie::Resources::Particles::Loop loop2(game.particlesHandler.particlesSystems[2].particlesEmiter[0].generators);
        //game.particlesHandler.particlesSystems[2].particlesEmiter[0].updates.push_back(&loop2);

        game.particlesHandler.particlesSystems[3].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(10, 5, 10));
        Cookie::Resources::Particles::SpherePositionGenerate circle4(Vec3(0, 0, 0), 0.5f, &game.particlesHandler.particlesSystems[3].trs.TRS);
        game.particlesHandler.particlesSystems[3].particlesEmiter[0].generators.push_back(&circle4);
        Cookie::Resources::Particles::Loop loop3(game.particlesHandler.particlesSystems[3].particlesEmiter[0].generators);
        //game.particlesHandler.particlesSystems[3].particlesEmiter[0].updates.push_back(&loop3);

        game.particlesHandler.particlesSystems[4].trs.TRS = Cookie::Core::Math::Mat4::Translate(Vec3(-10, 5, -10));
        Cookie::Resources::Particles::SpherePositionGenerate circle5(Vec3(0, 0, 0), 0.4f, &game.particlesHandler.particlesSystems[4].trs.TRS);
        game.particlesHandler.particlesSystems[4].particlesEmiter[0].generators.push_back(&circle5);
        Cookie::Resources::Particles::Loop loop4(game.particlesHandler.particlesSystems[4].particlesEmiter[0].generators);*/
        //game.particlesHandler.particlesSystems[4].particlesEmiter[0].updates.push_back(&loop4);

        for (int i = 0; i < game.particlesHandler.particlesSystems[0].particlesEmiter[0].generators.size(); i++)
            game.particlesHandler.particlesSystems[0].particlesEmiter[0].generators[i]->
            generate(&game.particlesHandler.particlesSystems[0].data[0], 0,
                game.particlesHandler.particlesSystems[0].data[0].countAlive);

        /*for (int i = 0; i < game.particlesHandler.particlesSystems[1].particlesEmiter[0].generators.size(); i++)
            game.particlesHandler.particlesSystems[1].particlesEmiter[0].generators[i]->
            generate(&game.particlesHandler.particlesSystems[1].data[0], 0,
                game.particlesHandler.particlesSystems[1].data[0].countAlive);

        for (int i = 0; i < game.particlesHandler.particlesSystems[2].particlesEmiter[0].generators.size(); i++)
            game.particlesHandler.particlesSystems[2].particlesEmiter[0].generators[i]->
            generate(&game.particlesHandler.particlesSystems[2].data[0], 0,
                game.particlesHandler.particlesSystems[2].data[0].countAlive);

        for (int i = 0; i < game.particlesHandler.particlesSystems[3].particlesEmiter[0].generators.size(); i++)
            game.particlesHandler.particlesSystems[3].particlesEmiter[0].generators[i]->
            generate(&game.particlesHandler.particlesSystems[3].data[0], 0,
                game.particlesHandler.particlesSystems[3].data[0].countAlive);

        for (int i = 0; i < game.particlesHandler.particlesSystems[4].particlesEmiter[0].generators.size(); i++)
            game.particlesHandler.particlesSystems[4].particlesEmiter[0].generators[i]->
            generate(&game.particlesHandler.particlesSystems[4].data[0], 0,
                game.particlesHandler.particlesSystems[4].data[0].countAlive);*/

        //Second Particles in the center particles 
       /* Cookie::Resources::Particles::ParticlesSystem second = Cookie::Resources::Particles::ParticlesSystem(40, 30);
        second.data[0].countAlive = 10;
        second.data[0].SetIsBIllboard(true);
        second.data[0].mesh = game.resources.meshes["Quad"].get();
        second.data[0].texture = game.resources.textures2D["Assets/Particles/Light.png"].get();
        Cookie::Resources::Particles::VelocityRandGenerate      velRand2(Vec3(-5, 5, -5), Vec3(5, 15, 5));
        Cookie::Resources::Particles::ScaleRandGenerate      scaleRand(Vec3(0.5, 0.5, 0.5), Vec3(1, 1, 1));
        second.particlesEmiter[0].generators.push_back(&box);
        second.particlesEmiter[0].generators.push_back(&velRand2);
        second.particlesEmiter[0].generators.push_back(&mass);
        second.particlesEmiter[0].generators.push_back(&timeRand);
        second.particlesEmiter[0].generators.push_back(&scaleRand);
        Cookie::Resources::Particles::ColorRandGenerate         yellow(Vec4(1, 0.5f, 0, 1), Vec4(1, 0.7, 0, 1));
        //Cookie::Resources::Particles::Loop              loop2(second.particlesEmiter[0].generators);
        second.particlesEmiter[0].generators.push_back(&yellow);
        for (int i = 0; i < second.particlesEmiter[0].generators.size(); i++)
            second.particlesEmiter[0].generators[i]->generate(&second.data[0], 0, second.data[0].countAlive);

        Cookie::Resources::Particles::ColorOverLife     col2(Vec4(0, 0, 0, 0));
        second.particlesEmiter[0].updates.push_back(&enabledGravity);
        second.particlesEmiter[0].updates.push_back(&updateVel);
        second.particlesEmiter[0].updates.push_back(&updateTime);
        second.particlesEmiter[0].updates.push_back(&col2);
        second.particlesEmiter[0].updates.push_back(&updateScale);

        game.particlesHandler.particlesSystems[4].data.push_back(second.data[0]);
        game.particlesHandler.particlesSystems[4].particlesEmiter.push_back(second.particlesEmiter[0]);*/
    

    bool isActive = false;
    {
        game.scene->map.model.albedo = game.resources.textures2D["Assets/Floor_DefaultMaterial_BaseColor.png"].get();
    }

    //for (int i = 0; i < MAX_ENTITIES; i++)
    //{
    //    game.coordinator.AddEntity(MODEL | TRANSFORM, std::to_string(i));
    //    game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->entities[i].id).mesh = game.resources.meshes["Sphere"].get();
    //    game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->entities[i].id).albedo = game.resources.textures["Green"].get();
    //    game.coordinator.componentHandler->GetComponentModel(game.coordinator.entityHandler->entities[i].id).metallicRoughness = game.resources.textures["Blue"].get();
    //    game.coordinator.componentHandler->GetComponentTransform(game.coordinator.entityHandler->entities[i].id).pos = { -50.0f + ((float)std::rand() / (float)RAND_MAX) * 100.0f, (float)std::rand() / (float)RAND_MAX * 10.0f + 1.0f, -50.0f + ((float)std::rand() / (float)RAND_MAX) * 100.0f };
    //    game.coordinator.componentHandler->GetComponentTransform(game.coordinator.entityHandler->entities[i].id).trsHasChanged = true;
    //}

    while (!glfwWindowShouldClose(game.renderer.window.window))
    {
        // Present frame


        //Update for 3D Music
        FMOD_VECTOR temp = { cam.pos.x, cam.pos.y, cam.pos.z };
        Cookie::Resources::SoundManager::system->set3DListenerAttributes(0, &temp, nullptr, nullptr, nullptr);
        Cookie::Resources::SoundManager::system->update();

        //TEMP : TEST FOR 3D
        //if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
        //    Cookie::Resources::SoundManager::PlayMusic("Magic.mp3");
        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
            Cookie::Resources::SoundManager::SetPaused("Music.mp3", true);
        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_L) == GLFW_PRESS)
            Cookie::Resources::SoundManager::SetPaused("Music.mp3", false);

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

        if (!ImGui::GetIO().MouseDownDuration[0])
        {            
            Core::Math::Vec3 fwdRay = cam.pos + cam.MouseToWorldDir() * cam.camFar;
            rp3d::Ray ray({ cam.pos.x,cam.pos.y,cam.pos.z }, {fwdRay.x,fwdRay.y,fwdRay.z});
            physHandle.editWorld->raycast(ray,this);
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
        
        game.CalculateMousePosInWorld(cam);
        game.HandleGameplayInputs(dbgRenderer);
        game.ECSCalls(dbgRenderer);
        game.coordinator.armyHandler->Debug();

		if (isActive)
            game.particlesHandler.Update();
        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
            isActive = true;
        game.coordinator.ApplyComputeTrs();

        //Draw
        game.renderer.Draw(&cam, game,editorFBO);
		game.particlesHandler.Draw(cam);

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
