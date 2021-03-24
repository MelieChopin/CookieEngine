#include <reactphysics3d/reactphysics3d.h>
#include "Editor.hpp" 
#include "UIallIn.hpp"
#include "Serialization.hpp"


using namespace Cookie;

Editor::Editor()
    : ui(game.renderer) 
{
    game.resources.Load(game.renderer);
    game.renderer.AddFrameBuffer(game.resources);
    cam.SetProj(Core::Math::ToRadians(60.f), game.renderer.state.viewport.Width, game.renderer.state.viewport.Height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    cam.pos = { 0.f , 20.0f,30.0f };
    cam.rot = { Core::Math::ToRadians(30.0f) ,0.0f,0.0f };
    cam.ResetPreviousMousePos();
    cam.Update();
    cam.Deactivate();
    //scene = Editor::Scene(resources, coordinator);
    game.scene->InitCoordinator(game.coordinator);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_W);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_S);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_A);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_D);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_SPACE);
    ImGui::GetIO().AddInputCharacter(GLFW_KEY_LEFT_CONTROL);
    game.resources.textures["Pink"] = (std::make_shared<Resources::Texture>(game.renderer, "Pink", Core::Math::Vec4(1.0f, 0.5f, 0.5f, 1.0f)));
    game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"] = (std::make_shared<Resources::Texture>(game.renderer, "Assets/Floor_DefaultMaterial_BaseColor.png"));

    //Load default Scene
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/DefaultDuck.CAsset", game);

    game.scene = _scene;
    game.scene->InitCoordinator(game.coordinator);


    ui.AddItem(new UIwidget::SaveButton(game.scene), 0);

    
    ui.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    ui.AddWItem(new UIwidget::TextureEditor(game.renderer, game.resources), 1);

    ui.AddWItem(new UIwidget::FileExplorer(game.renderer, game, selectedEntity), 2);

    ui.AddWItem(new UIwidget::Inspector(game.resources, game.coordinator, selectedEntity), 2);

    ui.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);

    ui.AddWItem(new UIwidget::Console(CDebug, game.renderer), 2);

    ui.AddWItem(new UIwidget::DemoWindow, 3);


    UIwidget::Toolbar* toolbar = new UIwidget::Toolbar(game.renderer);
    ui.AddWindow(new UIwidget::Viewport(toolbar, game.renderer.window.window, game.renderer.GetLastFrameBuffer(), &cam, game.coordinator, selectedEntity));


}

Editor::~Editor()
{

}

void Editor::Loop()
{

    while (!glfwWindowShouldClose(game.renderer.window.window))
    {
        // Present frame
        CDebug.UpdateTime();
        
        game.resources.UpdateScriptsContent();
        game.coordinator.ApplyScriptUpdate();
        


        // Present frame
        if (isPlaying)
        {
            game.Update();
        }
        else
        {
            glfwPollEvents();
            game.TryResizeWindow();
            game.renderer.Clear();

            cam.Update();

            
        }

        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_H) == GLFW_PRESS)
            Resources::Serialization::Save::SaveScene(*game.scene);

    
        game.renderer.Draw(cam.GetViewProj(), game.coordinator);
        game.renderer.SetBackBuffer();

        ui.UpdateUI();

        game.renderer.Render();
    }
}