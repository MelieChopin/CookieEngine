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

    while (!glfwWindowShouldClose(window.window))
    {
        Core::UpdateTime();

        // Present frame
        glfwPollEvents();

        TryResizeWindow();

        renderer.Clear();
        renderer.ClearFrameBuffer(frameBuffer);
        renderer.SetFrameBuffer(frameBuffer);

        camera->Update();

        //----------------COLLISION-------------------------------
        //Cookie::Core::Math::Vec3 firstPoint = camera.pos;
        //Cookie::Core::Math::Vec4 view = camera.GetViewProj().c[2];
        //Cookie::Core::Math::Vec3 cameraTarget{ view.x, view.y, view.z };
        //Cookie::Core::Math::Vec3 secondPoint = camera.pos + cameraTarget * 50;
        //Cookie::Core::Math::Vec3 result;
        //bool hit = scene[0].LinePlane(result, firstPoint, secondPoint);
       /* if (hit == true)
            std::cout << "hit" << " \n";
        else
            std::cout << "non \n";*/
        //--------------------------------------------------------

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
            

        //std::cout << scene.size() << "\n";

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