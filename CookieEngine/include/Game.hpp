#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Render/Renderer.hpp" 

#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Prefab.hpp"
#include "Resources/Sound.hpp"

#include "Resources/ResourcesManager.hpp"
#include "Drawers/Skybox.hpp"
#include "ECS/Coordinator.hpp"
#include "Resources/Particles/ParticlesHandler.hpp"
#include "PlayerData.hpp"

#include <memory>

namespace Cookie
{
	namespace Resources
	{
		class Scene;
	}

	class Game
	{
		private:
		public:
			Gameplay::PlayerData                            playerData;
			Render::Renderer								renderer;
			Resources::ResourcesManager						resources;
			Render::SkyBox									skyBox;
			ECS::Coordinator								coordinator;
			Render::FrameBuffer								frameBuffer;
			Resources::Particles::ParticlesHandler			particlesHandler;

			std::shared_ptr<Resources::Scene>				scene{nullptr};

		public:
			 Game();
			~Game();

			void Update();
			void Loop();

			//In Loop
			//Remove the debugRenderer later on
			void CalculateMousePosInWorld(Render::FreeFlyCam& cam);
			void HandleGameplayInputs(Render::DebugRenderer& dbg);
			void CheckIfBuildingValid();
			void InputCancelBuilding();
			void InputValidateBuilding();
			void InputStartSelectionQuad();
			void InputEndSelectionQuad();
			void InputMoveSelected();
			void InputSetNewEntityDestination();
			void InputStartBuilding(int index);
			void InputAddUnit(int index);
			void DisplayNewEntityDestination(Render::DebugRenderer& dbg);
			void ECSCalls(Render::DebugRenderer& dbg);

			void SetScene(const std::shared_ptr<Resources::Scene>& _scene);
			void TryResizeWindow();
	};
}

#endif // !__GAME_HPP__
