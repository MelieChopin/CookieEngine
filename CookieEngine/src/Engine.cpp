#include "Engine.hpp"
#include "Time.hpp"
#include "Debug.hpp"
#include "Core/Math/Calc.hpp"
#include "ImGui/imgui.h"
#include <vector>

using namespace Cookie;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;

Engine::Engine() :
    window{}, renderer{ window }, ui{ window.window, renderer }, frameBuffer{ resources,renderer }
{
    resources.Load(renderer);
    camera = std::make_shared<Render::GameCam>();
    camera->SetProj(Core::Math::ToRadians(60.f), renderer.state.viewport.Width,renderer.state.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    camera->pos = { 0.0f,20.0f,30.0f };
    camera->rot = { Core::Math::ToRadians(30.f) ,0.0f,0.0f};
    camera->ResetPreviousMousePos();
    camera->Update();
    camera->Deactivate();
    scene.reserve(MaxScene);
    scene.push_back(Editor::Scene(resources));
    scene.push_back(Editor::Scene(resources));
    scene[0].LoadScene(coordinator);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_W);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_S);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_A);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_D);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_SPACE);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_LEFT_CONTROL);
    resources.AddTexture(std::make_shared<Resources::Texture>(renderer, "Pink", Core::Math::Vec4(1.0f,0.5f,0.5f,1.0f)));
    resources.AddTexture(std::make_shared<Resources::Texture>(renderer, "Assets/Floor_DefaultMaterial_BaseColor.png"));
    coordinator.componentHandler->componentModels[0].texture = resources.GetTexture("Assets/Floor_DefaultMaterial_BaseColor.png");
}

Engine::~Engine()
{

}

/*
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << key << std::endl;

    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        std::cout << "Space Key Pressed" << std::endl;
    }
}*/

void Engine::Run()
{
    //Map in future Classes
    std::unordered_map<int, std::function<void()> > UnitInputs;
    UnitInputs['A'] = [] { std::cout << "Unit Shortcut 1\n"; };
    UnitInputs['Z'] = [] { std::cout << "Unit Shortcut 2\n"; };
    UnitInputs['E'] = [] { std::cout << "Unit Shortcut 3\n"; };
    std::unordered_map<int, std::function<void()> > BuildingInputs;
    BuildingInputs['A'] = [] { std::cout << "Building Shortcut 1\n"; };
    BuildingInputs['Z'] = [] { std::cout << "Building Shortcut 2\n"; };
    BuildingInputs['E'] = [] { std::cout << "Building Shortcut 3\n"; };

    input.Set(UnitInputs);

    ui.AddWItem(new UIwidget::ExitPannel(window.window), 0);
    ui.AddWItem(new UIwidget::FileExplorer, 2);
    
    UIwidget::Inspector* insp = new UIwidget::Inspector(resources, coordinator);
    ui.AddWItem(insp, 2);
    ui.AddWItem(new UIwidget::Hierarchy(resources, &scene, coordinator, insp), 2);
    
    ui.AddWindow(new UIwidget::Viewport(window.window, frameBuffer, &camera));
    //ui.AddWindow(new UIwidget::GamePort);

    ui.AddWItem(new UIwidget::Console(CDebug), 2);

    //Create default Ducks
    {
        coordinator.AddEntity(SIGNATURE_ALL_COMPONENT, resources, "Duck 1");
        ComponentTransform& trs1 = coordinator.componentHandler->GetComponentTransform(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id);
        trs1.localTRS.translation = { 10, 0, 0 };
        trs1.localTRS.scale = { 0.02, 0.02, 0.02 };
        coordinator.componentHandler->GetComponentRigidBody(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).speed = 10;
        ComponentModel& model1 = coordinator.componentHandler->GetComponentModel(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id);
        model1.mesh = resources.GetMesh("LOD3spShape");
        model1.texture = resources.GetTexture("Duck");

        
        coordinator.AddEntity(SIGNATURE_ALL_COMPONENT, resources, "Duck 2");
        ComponentTransform& trs2 = coordinator.componentHandler->GetComponentTransform(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id);
        trs2.localTRS.translation = { -10, 0, 0 };
        trs2.localTRS.scale = { 0.02, 0.02, 0.02 };
        coordinator.componentHandler->GetComponentRigidBody(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).speed = 10;
        coordinator.componentHandler->GetComponentModel(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).mesh = resources.GetMesh("LOD3spShape");
        ComponentModel& model2 = coordinator.componentHandler->GetComponentModel(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id);
        model2.mesh = resources.GetMesh("LOD3spShape");
        model2.texture = resources.GetTexture("Duck");
    }

    static bool camClicked = false;

    while (!glfwWindowShouldClose(window.window))
    {
        // Present frame
        Core::UpdateTime();

        // Present frame
        glfwPollEvents();
        TryResizeWindow();

        renderer.Clear();
        renderer.ClearFrameBuffer(frameBuffer);
        renderer.SetFrameBuffer(frameBuffer);

        if (glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_MIDDLE) && !camClicked)
        {
            Core::Math::Vec3 pos = camera->pos;
            Core::Math::Vec2 window = { { camera->width,camera->height } };
            if (isFlyCam)
            {
                camera = std::make_shared<Render::GameCam>();
                camera->rot = { Core::Math::ToRadians(30.f) ,0.0f,0.0f };
            }
            else
            {
                Core::Math::Vec3 rot = camera->rot;
                camera = std::make_shared<Render::FreeFlyCam>();
                camera->rot = rot;
            }

            camera->pos = pos;
            isFlyCam = !isFlyCam;
            camera->ResetPreviousMousePos();
            camera->SetProj(Core::Math::ToRadians(60.f),window.x,window.y, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
            camera->Update();
            camera->Deactivate();
            camClicked = true;
        }
        else if (!glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_MIDDLE) && camClicked)
        {
            camClicked = false;
        }

        camera->Update();

       //select unit or move selected
       if (glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_LEFT))
       {        
           //second condition not inside first "if" to not calculate ViewProj each frame
           Vec3 result;

           if (scene[0].LinePlane(result, camera->pos, camera->pos + camera->MouseToWorldDir() * camera->camFar))
           {
               //move to
               if (glfwGetKey(window.window, GLFW_KEY_LEFT_CONTROL) && coordinator.selectedEntity)
               {
                   ComponentRigidBody& rb = coordinator.componentHandler->GetComponentRigidBody(coordinator.selectedEntity->id);
                   rb.targetPosition = {result.x, coordinator.componentHandler->GetComponentTransform(coordinator.selectedEntity->id).localTRS.translation.y, result.z};
                   rb.goTowardTarget = true;
               }
               //select entity
               else
               {
                   int prevEntity = coordinator.selectedEntity ? coordinator.selectedEntity->id : -1;
                   coordinator.SelectClosestMovableEntity(result);
                   
                   if (coordinator.selectedEntity && prevEntity != coordinator.selectedEntity->id)
                   {

                       insp->SelectEntity(coordinator.selectedEntity);
                       ComponentModel& model = coordinator.componentHandler->GetComponentModel(coordinator.selectedEntity->id);
                       model.texture = resources.GetTexture("Pink");

                       if (prevEntity != -1)
                       {
                           ComponentModel& prevModel = coordinator.componentHandler->GetComponentModel((unsigned int)prevEntity);
                           prevModel.texture = resources.GetTexture("Duck");
                       }
                   }
               }
           }
       }

        coordinator.ApplySystemVelocity();
        coordinator.ApplyDraw(renderer.remote, camera->GetViewProj());
        renderer.SetBackBuffer();
        //frameBuffer.Draw(renderer.remote);
        
        ui.UpdateUI();

        renderer.Render();
    }
}


void Engine::TryResizeWindow()
{
    int width = 0;
    int height = 0;

    glfwGetWindowSize(window.window, &width, &height);

    if (width <= 0 || height <= 0)
        return;

    if (window.width != width || window.height != height)
    {
        Core::DebugMessageHandler::Summon().Log((std::to_string(width) + ' '+ std::to_string(height)).c_str());
        printf("%d, %d\n", width, height);
        window.width = width;
        window.height = height;

        renderer.ResizeBuffer(width,height);
        frameBuffer.Resize(renderer);
        camera->SetProj(Core::Math::ToRadians(60.f),width,height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}