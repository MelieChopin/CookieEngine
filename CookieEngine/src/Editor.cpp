#include <reactphysics3d/reactphysics3d.h>
#include "Editor.hpp" 
#include "UIallIn.hpp"
#include "Serialization.hpp"
#include "Physics/PhysicsHandle.hpp"

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
    game.resources.textures["Pink"] = (std::make_shared<Resources::Texture>("Pink", Core::Math::Vec4(1.0f, 0.5f, 0.5f, 1.0f)));
    game.resources.textures["Assets/Floor_DefaultMaterial_BaseColor.png"] = (std::make_shared<Resources::Texture>("Assets/Floor_DefaultMaterial_BaseColor.png"));

    //Load all prefabs in folder Prefabs
    Resources::Serialization::Load::LoadAllPrefabs(game.resources);

    //Load default Scene
    std::shared_ptr<Resources::Scene> _scene = Resources::Serialization::Load::LoadScene("Assets/Save/DefaultDuck.CAsset", game);

    game.SetScene(_scene);

    ui.AddItem(new UIwidget::SaveButton(game.scene, game.resources), 0);

    
    ui.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    ui.AddWItem(new UIwidget::TextureEditor(game.renderer, game.resources), 1);

    ui.AddWItem(new UIwidget::FileExplorer(game.renderer, game), 2);

    ui.AddWItem(new UIwidget::Inspector(selectedEntity, game.resources, game.coordinator, game.scene->physSim), 2);

    ui.AddWItem(new UIwidget::Hierarchy(game.resources, game.scene, game.coordinator, selectedEntity), 2);

    ui.AddWItem(new UIwidget::Console(CDebug, game.renderer), 2);

    ui.AddWItem(new UIwidget::DemoWindow, 3);


    UIwidget::Toolbar* toolbar = new UIwidget::Toolbar(game.renderer);
    ui.AddWindow(new UIwidget::Viewport(toolbar, game.renderer.window.window, game.renderer.GetLastFrameBuffer(), &cam, game.coordinator, selectedEntity));


    for (int i = 0; i <= game.coordinator.entityHandler->livingEntities; i++)
    {
        ECS::Entity& iEntity = game.coordinator.entityHandler->entities[i];
        if (iEntity.signature & SIGNATURE_PHYSICS)
        {
            ECS::ComponentPhysics& iPhysics = game.coordinator.componentHandler->componentPhysics[i];
            game.coordinator.componentHandler->componentTransforms[i].SetPhysics();
            iPhysics.physBody = game.scene->physSim.worldSim->createRigidBody(game.coordinator.componentHandler->componentTransforms[i].physTransform);
            iPhysics.physBody->setType(rp3d::BodyType::DYNAMIC);

            iPhysics.AddSphereCollider(2.0f, { 0.5f,1.5f,0.0f }, {0.0f,0.0f,0.0f});
        }
    }

    Physics::PhysicsHandle::editWorld = Physics::PhysicsHandle::physCom->createPhysicsWorld();
    InitEditComp();
    dbgRenderer.SetPhysicsRendering();
    Physics::PhysicsHandle::physSim->update(1.0e-7f);
    Physics::PhysicsHandle::editWorld->update(1.0e-7f);

}

Editor::~Editor()
{
    //Save all prefabs in folder Prefabs
    Resources::Serialization::Save::SaveAllPrefabs(game.resources);
}


void Editor::InitEditComp()
{
    for (int i = 1; i < MAX_ENTITIES; i++)
    {
        editingComponent[i].InitComponent(game.coordinator.componentHandler->GetComponentTransform(i).localTRS);
    }
}

void Editor::ModifyEditComp()
{
    for (int i = 1; i < MAX_ENTITIES; i++)
    {
        if ((game.coordinator.entityHandler->entities[i].signature & SIGNATURE_MODEL) == SIGNATURE_MODEL)
        {
            editingComponent[i].AABB = game.coordinator.componentHandler->GetComponentModel(i).mesh->AABBhalfExtent;
            editingComponent[i].MakeCollider();
        }
        editingComponent[i].editTrs = &game.coordinator.componentHandler->GetComponentTransform(i).localTRS;
        editingComponent[i].Update();
    }
}

void Editor::Loop()
{
    Physics::PhysicsHandle physHandle;
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

        if (physHandle.physSim->getIsDebugRenderingEnabled() != dbgRenderer.showDebug)
        {
            dbgRenderer.SetPhysicsRendering();
        }

        if (currentScene != game.scene.get())
        {
            if (selectedEntity.editComp)
            {
                selectedEntity.editComp->editTrs = &game.coordinator.componentHandler->GetComponentTransform(selectedEntity.focusedEntity->id).localTRS;
                selectedEntity.editComp->Update();
                if ((selectedEntity.focusedEntity->signature & SIGNATURE_MODEL) == SIGNATURE_MODEL)
                {
                    selectedEntity.editComp->AABB = game.coordinator.componentHandler->GetComponentModel(selectedEntity.focusedEntity->id).mesh->AABBhalfExtent;
                    selectedEntity.editComp->MakeCollider();
                }
            }
            selectedEntity = {};
            selectedEntity.componentHandler = game.coordinator.componentHandler;
            ModifyEditComp();
            currentScene = game.scene.get();
        }

        Physics::PhysicsHandle::editWorld->update(1.0e-7f);

        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_P) == GLFW_PRESS)
            Resources::Serialization::Save::SaveScene(*game.scene, game.resources);

        ////TEMP
        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_H) == GLFW_PRESS)
        {
            std::string duck = "Duck";
            game.coordinator.componentHandler->ModifyComponentOfEntityToPrefab(game.coordinator.entityHandler->entities[1], game.resources, duck);
        }

        if (!ImGui::GetIO().MouseDownDuration[0])
        {
            
            Core::Math::Vec3 fwdRay = cam.pos + cam.MouseToWorldDir() * cam.camFar;
            rp3d::Ray ray({ cam.pos.x,cam.pos.y,cam.pos.z }, {fwdRay.x,fwdRay.y,fwdRay.z});
            physHandle.editWorld->raycast(ray,this);
        }

        if (selectedEntity.toChangeEntityId >= 0)
        {
            PopulateFocusedEntity();
        }
            
        /////


        game.renderer.Draw(cam.GetViewProj(), game.coordinator);

        dbgRenderer.Draw(cam.GetViewProj());

        game.renderer.SetBackBuffer();

        ui.UpdateUI();

        game.renderer.Render();
    }
}
