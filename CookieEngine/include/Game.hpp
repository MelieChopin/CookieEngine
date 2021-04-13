#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <memory>
#include "Resources/ResourcesManager.hpp"
#include "Render/Renderer.hpp" 
#include "ECS/Coordinator.hpp"

namespace Cookie
{
	class Game
	{
		private:
		public:
			Render::Renderer			renderer;
			Resources::ResourcesManager resources;
			ECS::Coordinator			coordinator;

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
