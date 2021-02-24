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
    camera = std::make_shared<Render::FreeFlyCam>();
    camera->SetProj(Core::Math::ToRadians(60.f), renderer.state.viewport.Width / renderer.state.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    camera->pos = { 0.0f,0.0f,5.0f };
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
    ui.AddWindow(new UIwidget::FileExplorer);
    
    UIwidget::Inspector* insp = new UIwidget::Inspector(coordinator, resources);
    ui.AddWindow(insp);
    ui.AddWindow(new UIwidget::Hierarchy(coordinator, insp, resources));
    
    ui.AddWindow(new UIwidget::Viewport(window.window, frameBuffer, &camera));
    //ui.AddWindow(new UIwidget::GamePort);

    ui.AddWindow(new UIwidget::Console(Core::Debug::Summon()));

    static bool start = false;

    //Create default Ducks
    {
        coordinator.AddEntity(SIGNATURE_ALL_COMPONENT, resources, "Duck 1");
        ComponentTransform& trs1 = coordinator.componentHandler->GetComponentTransform(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id);
        trs1.localTRS.translation = { 10, 0, 0 };
        trs1.localTRS.scale = { 0.02, 0.02, 0.02 };
        coordinator.componentHandler->GetComponentRigidBody(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).speed = 10;
        coordinator.componentHandler->GetComponentModel(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).mesh = resources.GetMesh("LOD3spShape");
        
        coordinator.AddEntity(SIGNATURE_ALL_COMPONENT, resources, "Duck 2");
        ComponentTransform& trs2 = coordinator.componentHandler->GetComponentTransform(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id);
        trs2.localTRS.translation = { -10, 0, 0 };
        trs2.localTRS.scale = { 0.02, 0.02, 0.02 };
        coordinator.componentHandler->GetComponentRigidBody(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).speed = 10;
        coordinator.componentHandler->GetComponentModel(coordinator.entityHandler->entities[coordinator.entityHandler->livingEntities - 1].id).mesh = resources.GetMesh("LOD3spShape");
    }

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

        camera->Update();

       //select unit or move selected
       if (glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_LEFT))
       {        
           //second condition not inside first "if" to not calculate ViewProj each frame
           Vec4 view = camera->GetViewProj().c[2];
           Vec3 result;

           if (scene[0].LinePlane(result, camera->pos, camera->pos + Vec3{ view.x, view.y, view.z } *1000))
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
                   coordinator.SelectClosestMovableEntity(result);
           }
       }

       if(coordinator.selectedEntity)
           std::cout << coordinator.selectedEntity->name << std::endl;
       else
           std::cout << "no selected" << std::endl;

        //Scene
        {
            if (glfwGetKey(window.window, GLFW_KEY_P) == GLFW_PRESS)
                scene[1].LoadScene(coordinator);
            if (glfwGetKey(window.window, GLFW_KEY_K) == GLFW_PRESS)
                scene[0].LoadScene(coordinator);
            if (glfwGetKey(window.window, GLFW_KEY_U) == GLFW_PRESS)
                start = false;
            if (glfwGetKey(window.window, GLFW_KEY_O) == GLFW_PRESS && start == false)
            {
                if (scene.size() < MaxScene)
                    scene.push_back(Editor::Scene(resources));
                else
                    std::cout << "OUT OF RANGE\n";
                std::cout << scene.size() << "\n";
                start = true;
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

    if (window.width != width || window.height != height)
    {
        Core::Debug::Summon().Log((std::to_string(width) + ' '+ std::to_string(height)).c_str());
        printf("%d, %d\n", width, height);
        window.width = width;
        window.height = height;

        renderer.ResizeBuffer(width,height);
        frameBuffer.Resize(renderer);
        camera->SetProj(Core::Math::ToRadians(60.f),(float)width/(float)height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}