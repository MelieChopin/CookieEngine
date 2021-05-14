#ifndef __GAME_HPP__
#define __GAME_HPP__

#include "Render/Renderer.hpp" 
#include "Resources/Mesh.hpp"
#include "Resources/Texture.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Render/Skybox.hpp"
#include "ECS/Coordinator.hpp"
#include "Resources/Particles/ParticlesHandler.hpp"

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

			void SetScene(const std::shared_ptr<Resources::Scene>& _scene);
			void TryResizeWindow();
	};
}

#endif // !__GAME_HPP__
