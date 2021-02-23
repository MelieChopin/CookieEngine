#include "Engine.hpp"
#include "Time.hpp"
#include "Debug.hpp"
#include "Core/Math/Calc.hpp"
#include "ImGui/imgui.h"

using namespace Cookie;

Engine::Engine() :
    window{}, renderer{ window }, ui{ window.window, renderer }, frameBuffer{ coordinator.resources,renderer }
{
    coordinator.resources.Load(renderer);
    camera.SetProj(Core::Math::ToRadians(60.f), renderer.state.viewport.Width / renderer.state.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
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

    ui.AddWItem(new UIwidget::ExitPannel(window.window), 0);
    ui.AddWindow(new UIwidget::FileExplorer);
    
    UIwidget::Inspector* insp = new UIwidget::Inspector(coordinator);
    ui.AddWindow(insp);
    ui.AddWindow(new UIwidget::Hierarchy(coordinator, insp));
    
    ui.AddWindow(new UIwidget::Viewport(window.window, frameBuffer, ui.mouseCaptured));
    //ui.AddWindow(new UIwidget::GamePort);

    ui.AddWindow(new UIwidget::Console(Core::Debug::Summon()));

    Core::Debug::Summon().Log("This is a Log.");
    Core::Debug::Summon().Warning("This is a warning!");
    Core::Debug::Summon().Error("This is an error!!");
    Core::Debug::Summon().Exception("This is an exception!!!");

    while (!glfwWindowShouldClose(window.window))
    {
        Core::UpdateTime();

        // Present frame
        glfwPollEvents();

        TryResizeWindow();

        renderer.Clear();
        renderer.ClearFrameBuffer(frameBuffer);
        renderer.SetFrameBuffer(frameBuffer);

        if(ui.mouseCaptured)
            camera.UpdateFreeFly(window.window);


        coordinator.ApplySystemVelocity();
        coordinator.ApplyDraw(renderer.remote, camera.GetViewProj());
        renderer.SetBackBuffer();
        //frameBuffer.Draw(renderer.remote);
        
        ui.UpdateUI();

        renderer.Render();
    }
}


void Engine::TryResizeWindow()
{
    int width, height = 0;

    glfwGetWindowSize(window.window, &width, &height);

    if (window.width != width || window.height != height)
    {
        window.width = width;
        window.height = height;

        renderer.ResizeBuffer(width,height);
        camera.SetProj(Core::Math::ToRadians(60.f),(float)width/(float)height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}