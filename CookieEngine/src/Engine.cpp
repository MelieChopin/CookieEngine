#include "Engine.hpp"

using namespace Cookie;

Engine::Engine() :
    window{}, renderer{window}
{

}

Engine::~Engine()
{

}

void Engine::Run()
{
    while (!glfwWindowShouldClose(window.window))
    {
        // Present frame
        glfwPollEvents();

        renderer.Clear();

        renderer.Render();
    }
}