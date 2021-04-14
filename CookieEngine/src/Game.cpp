#include <reactphysics3d/reactphysics3d.h>
#include "Game.hpp"

using namespace Cookie;

/*================== CONSTRUCTORS/DESTRUCTORS ==================*/

Game::Game():
    skyBox{resources}
{
    resources.textures["SkyBoxTex"] = std::make_shared<Resources::Texture>("Assets/SkyBox01.dds");
    skyBox.texture = resources.textures["SkyBoxTex"];
}

Game::~Game()
{
}

/*================== LOOP ==================*/

void Game::Loop()
{

    while (!glfwWindowShouldClose(renderer.window.window))
    {
        Update();
        renderer.Render();
    }
}

void Game::Update()
{
    // Present frame
    glfwPollEvents();
    TryResizeWindow();

    //physSim.Update();
    //coordinator.ApplySystemPhysics(physSim.factor);

    renderer.Clear();

    scene->camera->Update();
    coordinator.ApplyScriptUpdate();

    renderer.Draw(scene->camera.get(), *this);
    renderer.SetBackBuffer();
}


/*================== SETTER/GETTER ==================*/

void Game::SetScene(const std::shared_ptr<Resources::Scene>& _scene)
{
    scene = _scene;
    scene->InitCoordinator(coordinator);
}

void Game::TryResizeWindow()
{
    int width = 0;
    int height = 0;

    glfwGetWindowSize(renderer.window.window, &width, &height);

    if (width <= 0 || height <= 0)
        return;

    if (renderer.window.width != width || renderer.window.height != height)
    {
        Core::DebugMessageHandler::Summon().Log((std::to_string(width) + ' ' + std::to_string(height)).c_str());
        printf("%d, %d\n", width, height);
        renderer.window.width = width;
        renderer.window.height = height;

        renderer.ResizeBuffer(width, height);
        //scene->camera->SetProj(Core::Math::ToRadians(60.f), width, height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}