#include <cstdio>
#include "Core/Window.hpp"

using namespace Cookie::Core;

constexpr int initWidth = 1280;
constexpr int initHeight = 720;


void debugGLFWCallback(int error_code, const char* description)
{
    printf("GLFW error (%x) = %s\n", error_code, description);
}

/*============== CONSTRUCTORS/DESTRUCTORS =================*/

Window::Window()
{
    // Init glfw
    if (!glfwInit())
    {
        fprintf(stderr, "glfwInit failed");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwSetErrorCallback(debugGLFWCallback);

    //create window
    window = glfwCreateWindow(initWidth, initHeight, "Cookie Engine", nullptr, nullptr);
    if (window == nullptr)
    {
        fprintf(stderr, "glfwCreateWindow failed");
    }
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}