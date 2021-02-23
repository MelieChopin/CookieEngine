#include "Engine.hpp"
#include "Time.hpp"
#include "Debug.hpp"
#include "Core/Math/Calc.hpp"
#include "ImGui/imgui.h"
#include <vector>

using namespace Cookie;

Engine::Engine() :
    window{}, renderer{ window }, ui{ window.window, renderer }, frameBuffer{ resources,renderer }
{
    resources.Load(renderer);
    camera.SetProj(Core::Math::ToRadians(60.f), renderer.state.viewport.Width / renderer.state.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    camera.Update();
    scene.push_back(Cookie::Editor::Scene());
    scene[indexScene].InitScene(resources);
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
    
    UIwidget::Inspector* insp = new UIwidget::Inspector(scene[indexScene].coordinator, resources);
    ui.AddWindow(insp);
    ui.AddWindow(new UIwidget::Hierarchy(scene[indexScene].coordinator, insp, resources));
    
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


        //----------------COLLISION-------------------------------
        Cookie::Core::Math::Vec3 firstPoint = camera.pos;
        Cookie::Core::Math::Vec4 view = camera.GetViewProj().c[2];
        Cookie::Core::Math::Vec3 cameraTarget{ view.x, view.y, view.z };
        Cookie::Core::Math::Vec3 secondPoint = camera.pos + cameraTarget * 50;
        Cookie::Core::Math::Vec3 result;
        bool hit = scene[indexScene].LinePlane(result, firstPoint, secondPoint);
        if (hit == true)
            std::cout << "hit" << " \n";
        else
            std::cout << "non \n";
        //--------------------------------------------------------

        scene[indexScene].coordinator.ApplySystemVelocity();
        scene[indexScene].coordinator.ApplyDraw(renderer.remote, camera.GetViewProj());
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