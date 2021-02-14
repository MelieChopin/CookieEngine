#include "Engine.hpp"

using namespace Cookie;

Engine::Engine()
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
    }
}