#include "Core/Primitives.hpp"
#include "Physics/PhysicsHandle.hpp"
#include "Resources/Scene.hpp"
#include "Game.hpp"
#include "ECS/ComponentGameplay.hpp"

#include "UIcore.hpp"

using namespace Cookie;
using namespace Cookie::Core::Math;
using namespace Cookie::ECS;
using namespace Cookie::Gameplay;
using namespace rp3d;

/*================== CONSTRUCTORS/DESTRUCTORS ==================*/

Game::Game():
    skyBox{resources},
    frameBuffer{renderer.window.width,renderer.window.height }
{

    Physics::PhysicsHandle::Init();
    Core::UIcore::FinishInit(renderer);
}

Game::~Game()
{
    Physics::PhysicsHandle::Terminate();
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
    coordinator.ApplyComputeTrs();

    renderer.Draw(scene->camera.get(), *this,frameBuffer);
    particlesHandler.Draw(*scene->camera.get());

    renderer.SetBackBuffer();
}

void Game::CalculateMousePosInWorld(Render::FreeFlyCam& cam)
{
    Core::Math::Vec3 fwdRay = cam.pos + cam.MouseToWorldDir() * cam.camFar;
    rp3d::Ray ray({ cam.pos.x,cam.pos.y,cam.pos.z }, { fwdRay.x,fwdRay.y,fwdRay.z });
    RaycastInfo raycastInfo;

    //if raycast hit
    if (scene->map.physic.physBody->raycast(ray, raycastInfo))
        playerData.mousePosInWorld = {raycastInfo.worldPoint.x, raycastInfo.worldPoint.y, raycastInfo.worldPoint.z};

}
void Game::HandleGameplayInputs()
{
    if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_N])
    {
        ECS::Entity& newEntity = coordinator.AddEntity(resources.prefabs["04Base"].get(), "good");

        ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(newEntity.id);
        CGPProducer& producer = coordinator.componentHandler->GetComponentGameplay(newEntity.id).componentProducer;

        trs.pos = scene->map.GetCenterOfBuilding(playerData.mousePosInWorld, producer.tileSize);			
        Vec3 posTopLeft = trs.pos - trs.scale / 2;
        scene->map.GiveTilesToBuilding(scene->map.GetTileIndex(posTopLeft), producer);        
    }
    if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_B])
    {
        ECS::Entity& newEntity = coordinator.AddEntity(resources.prefabs["04Base"].get(), "bad");

        ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(newEntity.id);
        CGPProducer& producer = coordinator.componentHandler->GetComponentGameplay(newEntity.id).componentProducer;

        trs.pos = scene->map.GetCenterOfBuilding(playerData.mousePosInWorld, producer.tileSize);
        Vec3 posTopLeft = trs.pos - trs.scale / 2;
        scene->map.GiveTilesToBuilding(scene->map.GetTileIndex(posTopLeft), producer);
    }
    if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_X])
    {
        ECS::Entity& newEntity = coordinator.AddEntity(resources.prefabs["Resource"].get(), "good");

        ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(newEntity.id); 
        Vec2 tileSize {{1, 1}};
        trs.pos = scene->map.GetCenterOfBuilding(playerData.mousePosInWorld, tileSize);
    }
    if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_I])
        coordinator.armyHandler->AddArmyCoordinator("bad");
        


    if (playerData.buildingToBuild)
    {
        CheckIfBuildingValid();

        if (!ImGui::GetIO().MouseDownDuration[0] && playerData.isBuildingValid)
            InputValidateBuilding();
        if (!ImGui::GetIO().MouseDownDuration[1])
            InputCancelBuilding();
    }
    else
    {
        if (ImGui::GetIO().MouseClicked[0])
            InputStartSelectionQuad();

        if (ImGui::GetIO().MouseReleased[0])
            InputEndSelectionQuad();

        if (!ImGui::GetIO().MouseDownDuration[1])
        {
            //CGPMove
            InputMoveSelected();
            
            //CGPProducer
            InputSetNewEntityDestination();

            //CGPWorker
            InputSetResourceToWorkers();
        }

        if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_C])
            InputStartBuilding(0);
        if (!ImGui::GetIO().KeysDownDuration[GLFW_KEY_V])
            InputAddUnit(0);
    }
}

void Game::CheckIfBuildingValid()
{
    playerData.buildingPos = scene->map.GetCenterOfBuilding(playerData.mousePosInWorld, playerData.buildingToBuild->tileSize);
        
    Vec3 posTopLeft =  {playerData.buildingPos.x - playerData.buildingToBuild->tileSize.x * scene->map.tilesSize.x / 2,
                        playerData.buildingPos.y,
                        playerData.buildingPos.z - playerData.buildingToBuild->tileSize.y * scene->map.tilesSize.y / 2};

    playerData.isBuildingValid = scene->map.isBuildingValid(scene->map.GetTileIndex(posTopLeft), playerData.buildingToBuild->tileSize);
}
void Game::InputCancelBuilding()
{
    playerData.buildingToBuild = nullptr;
    playerData.workerWhoBuild = nullptr;
    playerData.indexOfBuildingInWorker = 0;
    playerData.isBuildingValid = false;
}
void Game::InputValidateBuilding()
{
    playerData.workerWhoBuild->StartBuilding(playerData.buildingPos, playerData.indexOfBuildingInWorker);

    playerData.buildingToBuild = nullptr;
    playerData.workerWhoBuild = nullptr;
    playerData.indexOfBuildingInWorker = 0;
}
void Game::InputStartSelectionQuad()
{
    playerData.makingASelectionQuad = true;
    playerData.selectionQuadStart = playerData.mousePosInWorld;
}
void Game::InputEndSelectionQuad()
{
    playerData.makingASelectionQuad = false;

    //if selection quad is small try select closest Entity else select all entities with trs.pos inside selection quad
    if ((playerData.selectionQuadStart - playerData.mousePosInWorld).Length() < MINIMUM_SELECTION_QUAD_LENGTH)
    {
        coordinator.selectedEntities.clear();
        ECS::Entity* possibleSelectedEntity = coordinator.GetClosestSelectableEntity(playerData.selectionQuadStart);

        if (possibleSelectedEntity)
            coordinator.selectedEntities.push_back(possibleSelectedEntity);
    }
    else
        coordinator.SelectEntities(playerData.selectionQuadStart, playerData.mousePosInWorld);
}
void Game::InputMoveSelected()
{
    if (scene->map.GetTile(playerData.mousePosInWorld).isObstacle)
        return;

    //set MovableEntities
    std::vector<ECS::Entity*> movableEntities;
    for (int i = 0; i < coordinator.selectedEntities.size(); ++i)
    {
        float selectedEntityId = coordinator.selectedEntities[i]->id;
        ComponentGameplay& gameplay = coordinator.componentHandler->GetComponentGameplay(selectedEntityId);

        if (gameplay.signatureGameplay & CGP_SIGNATURE::MOVE)
            movableEntities.push_back(coordinator.selectedEntities[i]);
    }

    //Calculate centroid
    Vec3 centroid{ 0, 0, 0 };
    for (int i = 0; i < movableEntities.size(); ++i)
    {
        //divide by movableEntities.size in for loop, so we're sure we can't divide by 0 if we don't use a if (movableEntities.size())
        centroid += coordinator.componentHandler->GetComponentTransform(movableEntities[i]->id).pos / movableEntities.size();
    }

    //Apply pathfind to each movableEntities
    for (int i = 0; i < movableEntities.size(); ++i)
    {
        float movableEntitiesId = movableEntities[i]->id;
        ComponentGameplay& gameplay = coordinator.componentHandler->GetComponentGameplay(movableEntitiesId);
        ComponentTransform& trs = coordinator.componentHandler->GetComponentTransform(movableEntitiesId);

        Vec3 offsetFromCentroid = trs.pos - centroid;
        Vec3 finalPos = playerData.mousePosInWorld + offsetFromCentroid;
        //pathfind to mousePos + offset
        if (offsetFromCentroid.Length() < OFFSET_MAX_FROM_CENTROID && scene->map.ApplyPathfinding(scene->map.GetTile(trs.pos), scene->map.GetTile(finalPos)))
            gameplay.componentMove.SetPath(scene->map.GetTile(finalPos));
        //pathfind to mousePos
        else if (scene->map.ApplyPathfinding(scene->map.GetTile(trs.pos), scene->map.GetTile(playerData.mousePosInWorld)))
            gameplay.componentMove.SetPath(scene->map.GetTile(playerData.mousePosInWorld));
        else
            std::cout << "No Path Find\n";
    }

}
void Game::InputSetNewEntityDestination()
{
    for (int i = 0; i < coordinator.selectedEntities.size(); ++i)
    {
        float selectedEntityId = coordinator.selectedEntities[i]->id;
        ComponentGameplay& gameplay = coordinator.componentHandler->GetComponentGameplay(selectedEntityId);

        if (gameplay.signatureGameplay & CGP_SIGNATURE::PRODUCER)
            gameplay.componentProducer.newUnitDestination = playerData.mousePosInWorld;
    }
}
void Game::InputSetResourceToWorkers()
{
    ECS::Entity* resource = coordinator.GetClosestSelectableEntity(playerData.mousePosInWorld, CGP_SIGNATURE::RESOURCE);

    //if we don't select an entity with CGPResource in the first place, quit
    if (!resource)
        return;

    for (int i = 0; i < coordinator.selectedEntities.size(); ++i)
    {
        if (coordinator.componentHandler->GetComponentGameplay(resource->id).componentResource.nbOfWorkerOnIt == MAX_WORKER_PER_RESOURCE)
        {
            resource = coordinator.GetClosestFreeResourceEntity(playerData.mousePosInWorld);
            //if there is no more free resources available
            if (!resource)
                return;
        }

        float selectedEntityId = coordinator.selectedEntities[i]->id;
        ComponentGameplay& gameplay = coordinator.componentHandler->GetComponentGameplay(selectedEntityId);

        if (gameplay.signatureGameplay & CGP_SIGNATURE::WORKER)
            gameplay.componentWorker.SetResource(coordinator.componentHandler->GetComponentTransform(resource->id).pos, coordinator.componentHandler->GetComponentGameplay(resource->id).componentResource);

    }

}
void Game::InputStartBuilding(int index)
{
    for (int i = 0; i < coordinator.selectedEntities.size(); ++i)
    {
        ComponentGameplay& gameplay = coordinator.componentHandler->GetComponentGameplay(coordinator.selectedEntities[i]->id);

        if (gameplay.signatureGameplay & CGP_SIGNATURE::WORKER && !gameplay.componentWorker.BuildingInConstruction)
        {
            playerData.buildingToBuild = &gameplay.componentWorker.possibleBuildings[0]->gameplay.componentProducer;
            playerData.workerWhoBuild = &gameplay.componentWorker;
            playerData.indexOfBuildingInWorker = index;
            break;
        }
    }
}
void Game::InputAddUnit(int index)
{
    for (int i = 0; i < coordinator.selectedEntities.size(); ++i)
    {
        ComponentGameplay& gameplay = coordinator.componentHandler->GetComponentGameplay(coordinator.selectedEntities[i]->id);

        if (gameplay.signatureGameplay & CGP_SIGNATURE::PRODUCER)
            gameplay.componentProducer.AddUnitToQueue(index);
    }
}

void Game::ECSCalls(Render::DebugRenderer& dbg)
{
    coordinator.armyHandler->UpdateArmyCoordinators(scene->map);
    resources.UpdateScriptsContent();
    coordinator.ApplyScriptUpdate();
    coordinator.UpdateCGPProducer(scene->map);
    coordinator.UpdateCGPWorker(scene->map);
    coordinator.UpdateCGPMove(scene->map, dbg);
    coordinator.UpdateCGPAttack();
    coordinator.ApplyRemoveUnnecessaryEntities();
}

/*================== SETTER/GETTER ==================*/

void Game::SetScene(const std::shared_ptr<Resources::Scene>& _scene)
{
    scene = _scene;
    scene->InitCoordinator(coordinator);

    scene->camera = std::make_shared<Render::GameCam>();

    scene->camera->SetProj(110.f, renderer.window.width, renderer.window.height, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
    scene->camera->pos = { 0.f , 20.0f,15.0f };
    scene->camera->rot = { Core::Math::ToRadians(80.0f) ,0.0f,0.0f };
    scene->camera->ResetPreviousMousePos();
    scene->camera->mapClampX = {-scene->map.trs.scale.x*0.5f,scene->map.trs.scale.x * 0.5f};
    scene->camera->mapClampZ = {-scene->map.trs.scale.z*0.5f,scene->map.trs.scale.z * 0.5f};
    //scene->camera->Update();
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
