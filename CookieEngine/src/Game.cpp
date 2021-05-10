#include "Resources/Scene.hpp"
#include "Game.hpp"

#include "UIcore.hpp"

using namespace Cookie;

/*================== CONSTRUCTORS/DESTRUCTORS ==================*/

Game::Game():
    skyBox{resources},
    frameBuffer{renderer.window.width,renderer.window.height}
{
    Core::UIcore::FinishInit(renderer);
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
    renderer.ClearFrameBuffer(frameBuffer);

    scene->camera->Update();
    coordinator.ApplyScriptUpdate();

    renderer.Draw(scene->camera.get(), *this,frameBuffer);
    renderer.SetBackBuffer();
}


/*================== SETTER/GETTER ==================*/

void Game::SetScene(const std::shared_ptr<Resources::Scene>& _scene)
{
    scene = _scene;
    scene->InitCoordinator(coordinator);

    scene->camera = std::make_shared<Render::GameCam>();

    scene->camera->SetProj(Core::Math::ToRadians(60.f), renderer.window.width, renderer.window.height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    scene->camera->pos = { 0.f , 20.0f,30.0f };
    scene->camera->rot = { Core::Math::ToRadians(30.0f) ,0.0f,0.0f };
    scene->camera->ResetPreviousMousePos();
    scene->camera->Update();
    scene->camera->Deactivate();
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
        frameBuffer.Resize(width, height);
        //scene->camera->SetProj(Core::Math::ToRadians(60.f), width, height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    }
}
