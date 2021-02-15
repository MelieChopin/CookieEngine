#include "Engine.hpp"

using namespace Cookie;

Engine::Engine()
{

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

    while (!glfwWindowShouldClose(window.window))
    {
        // Present frame
        glfwPollEvents();        
        
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
}