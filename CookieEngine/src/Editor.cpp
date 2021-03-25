#include <reactphysics3d/reactphysics3d.h>
#include "Editor.hpp" 
#include "UIallIn.hpp"
#include "Serialization.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Map.hpp"


using namespace Cookie;
using namespace rp3d;

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

    game.SetScene(_scene);

    ui.AddItem(new UIwidget::SaveButton(game.scene), 0);

    
    ui.AddWItem(new UIwidget::ExitPannel(game.renderer.window.window), 0);

    ui.AddWItem(new UIwidget::TextureEditor(game.renderer, game.resources), 1);

    ui.AddWItem(new UIwidget::FileExplorer(game.renderer, game, selectedEntity), 2);

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

            iPhysics.AddSphereCollider(1.0f, { 0.0f,0.0f,0.0f }, {0.0f,0.0f,0.0f});
        }
    }

    game.scene->physSim.worldSim->setIsDebugRenderingEnabled(true);
    dbgRenderer.physicsDebug = &game.scene->physSim.worldSim->getDebugRenderer();
    dbgRenderer.physicsDebug->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLISION_SHAPE,true);
    dbgRenderer.physicsDebug->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
    dbgRenderer.physicsDebug->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    dbgRenderer.physicsDebug->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
    dbgRenderer.physicsDebug->setIsDebugItemDisplayed(rp3d::DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);
}

Editor::~Editor()
{

}

void Editor::Loop()
{
    /*
    ECS::ComponentPhysics	physic;
    physic.physBody = Physics::PhysicsHandle::physSim->createRigidBody(rp3d::Transform(rp3d::Vector3(0.0, 0.0, 0.0), rp3d::Quaternion::identity()));
    physic.physBody->setType(rp3d::BodyType::STATIC);
    physic.AddCubeCollider(Core::Math::Vec3(1000, 1, 1000), Core::Math::Vec3(0, 0, 0), Core::Math::Vec3(0, 0, 0));
    */


    while (!glfwWindowShouldClose(game.renderer.window.window))
    {
        // Present frame
        CDebug.UpdateTime();
        
        game.resources.UpdateScriptsContent();
        game.coordinator.ApplyScriptUpdate();

        /*
        // Create the ray
        Vector3 startPoint(cam.pos.x, cam.pos.y, cam.pos.z);
        Core::Math::Vec3 camDir = cam.MouseToWorldDir();
        Vector3 endPoint(camDir.x, camDir.y, camDir.z);
        endPoint = startPoint + endPoint * 1000;

        Ray ray(startPoint, endPoint);
        // Create the raycast info object for the
        // raycast result
        RaycastInfo raycastInfo;
        // Raycast test
        if(physic.physBody->raycast(ray, raycastInfo))
            std::cout << "Hit point : " <<
            raycastInfo.worldPoint.x << " " <<
            raycastInfo.worldPoint.y << " " <<
            raycastInfo.worldPoint.z << " " <<
            std::endl;
            */
        


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

        if (dbgRenderer.showDebug)
        {
            //game.scene->physSim.Update();
            //game.coordinator.ApplySystemPhysics(game.scene->physSim.factor);
            //rp3d::DebugRenderer& ren = game.scene->physSim.worldSim->getDebugRenderer();
            //rp3d::List<rp3d::DebugRenderer::DebugTriangle> triangles = ren.getTriangles();
            //
            //std::vector<rp3d::DebugRenderer::DebugTriangle> triArray;
            //triArray.resize(dbgRenderer.physicsDebug->getNbTriangles(), rp3d::DebugRenderer::DebugTriangle({ 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, {0}));
            //memcpy(triArray.data(), (void*)dbgRenderer.physicsDebug->getTrianglesArray(), sizeof(rp3d::DebugRenderer::DebugTriangle) * dbgRenderer.physicsDebug->getNbTriangles());
            //
            //
            //
            //for (int i = 0; i < triArray.size(); i++)
            //{
            //    float x = triArray[i].point1.x;
            //    (void)x;
            //}
            //
        }

        if (glfwGetKey(game.renderer.window.window, GLFW_KEY_H) == GLFW_PRESS)
            Resources::Serialization::Save::SaveScene(*game.scene);

        game.renderer.Draw(cam.GetViewProj(), game.coordinator);
        game.renderer.SetBackBuffer();

        ui.UpdateUI();

        game.renderer.Render();
    }
}