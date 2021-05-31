#ifndef __DRAW_DATA_HANDLER_HPP__
#define __DRAW_DATA_HANDLER_HPP__

#include <Mat4.hpp>
#include <array>
#include <vector>
#include "Drawers/MapDrawer.hpp"

struct ID3D11Buffer;
struct ID3D11DepthStencilView;

namespace Cookie
{
	class Game;

	namespace Gameplay
	{
		struct PlayerData;
	}

	namespace ECS
	{
		class Coordinator;
		class ComponentModel;
		class ComponentGameplay;
	}

	namespace Resources
	{
		class Texture;
		class Mesh;
	}

	namespace Render
	{
		class Camera;

		struct Frustrum
		{
			std::array<Core::Math::Vec4, 6> planes;
			Core::Math::Vec3				centroid;
			std::array<Core::Math::Vec3, 8> corners;

			void MakeFrustrum(const Camera& cam);
		};
		
		class DrawDataHandler
		{
			public:
				const ECS::Coordinator*				coordinator	{nullptr};
				ID3D11Buffer*						CBuffer		{nullptr};
				Frustrum							frustrum;

				std::vector<ECS::ComponentModel>	models;
				std::vector<Core::Math::Mat4>		matrices;
				std::array<Core::Math::Vec3, 2>		AABB;
				
				ID3D11DepthStencilView*				depthStencilView;
				ID3D11Buffer*						CamCBuffer;
				MapDrawer							mapDrawer;

				const Camera*						currentCam;
				const Gameplay::PlayerData*			player;


				std::vector<ECS::ComponentModel>	selectedModels;
				std::vector<Core::Math::Mat4>		selectedMatrices;
				std::vector<ECS::ComponentGameplay> selectedGameplays;

			private:
				void InitCBuffer();

			public:
				DrawDataHandler();
				~DrawDataHandler();

				void Init(const Game& game);
				void SetDrawData(const Camera* cam, const Game& game);
				void Draw(int _i = 0);
				void Clear();
		};
	}
}

#endif // !__DRAW_DATA_HANDLER_HPP__
