#include "Engine.hpp"
#include "Time.hpp"
#include "Debug.hpp"
#include "Core/Math/Calc.hpp"
#include "ImGui/imgui.h"

using namespace Cookie;

Engine::Engine() :
    window{}, renderer{window}, ui{window.window, renderer}
{
    coordinator.resources.Load(renderer);
    camera.SetProj(Core::Math::ToRadians(60.f), renderer.state.viewport.Width / renderer.state.viewport.Height, 0.01f, 1000.f);
    camera.pos = Core::Math::Vec3(0.0f, 50.0f, 200.0f);
    camera.Update();
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
    input.Set(VK_SPACE, [] { std::cout << "This shortcut won't change depending on the selected \n"; });
    //Map in future Classes
    std::map<int, std::function<void()> > UnitInputs;
    UnitInputs['A'] = [] { std::cout << "Unit Shortcut 1\n"; };
    UnitInputs['Z'] = [] { std::cout << "Unit Shortcut 2\n"; };
    UnitInputs['E'] = [] { std::cout << "Unit Shortcut 3\n"; };
    std::map<int, std::function<void()> > BuildingInputs;
    BuildingInputs['A'] = [] { std::cout << "Building Shortcut 1\n"; };
    BuildingInputs['Z'] = [] { std::cout << "Building Shortcut 2\n"; };
    BuildingInputs['E'] = [] { std::cout << "Building Shortcut 3\n"; };

    input.Set(UnitInputs);
    //glfwSetKeyCallback(window.window, keyCallback);
    //glfwSetInputMode(window.window, GLFW_STICKY_KEYS, 1);

    coordinator.AddEntity(SIGNATURE_MODEL + SIGNATURE_TRANSFORM,"Duck");
    ECS::ComponentModel model;
    model.mesh = coordinator.resources.GetMesh("LOD3spShape");
    model.shader = coordinator.resources.GetDefaultShader();
    coordinator.componentHandler.componentModels[0] = model;


    ui.AddWItem(new UIwidget::ExitPannel(window.window), 0);
    ui.AddWindow(new UIwidget::FileExplorer);
    
    UIwidget::Inspector* insp = new UIwidget::Inspector(coordinator);
    ui.AddWindow(insp);
    ui.AddWindow(new UIwidget::Hierarchy(coordinator, insp));
    
    ui.AddWindow(new UIwidget::Viewport);
    ui.AddWindow(new UIwidget::GamePort);

    while (!glfwWindowShouldClose(window.window))
    {
        // Present frame
        glfwPollEvents();

        renderer.Clear();     
        
        //Input Test
        {
            input.CheckInputs();
            if (GetAsyncKeyState('Q') & 0xff)
            {
                std::cout << "Unit Selected\n";
                input.Set(UnitInputs);
            }
            if (GetAsyncKeyState('S') & 0xff)
            {
                std::cout << "Building Selected\n";
                input.Set(BuildingInputs);
            }
            //if (glfwGetKey(window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            if (GetAsyncKeyState(VK_RBUTTON) & 0xff)
            {
                std::cout << "escape pressed\n";
            }
        }

        //ECS Test
        {
            if (GetAsyncKeyState('W') & 0xff)
            {
                std::cout << "Entity Created\n";
                coordinator.AddEntity(SIGNATURE_TRANSFORM + SIGNATURE_RIGIDBODY);
            }
            if (GetAsyncKeyState('X') & 0xff)
            {
                std::cout << "Entity Removed\n";
                coordinator.RemoveEntity(coordinator.entityHandler.entities[0]);
            }
            if (GetAsyncKeyState('C') & 0xff)
                coordinator.ApplySystemDisplayId();
        }

        coordinator.ApplyDraw(renderer.remote, camera.GetViewProj());
        ui.UpdateUI();

        renderer.Render();
    }
}
