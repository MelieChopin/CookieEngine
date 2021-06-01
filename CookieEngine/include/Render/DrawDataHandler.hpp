#ifndef __DRAW_DATA_HANDLER_HPP__
#define __DRAW_DATA_HANDLER_HPP__

#include <Mat4.hpp>
#include <array>
#include <vector>
#include "Drawers/ModelDrawer.hpp"
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
		class Scene;
		class Texture;
		class Mesh;
	}

	namespace Render
	{
		class Camera;
		struct LightsArray;

		struct Frustrum
		{
			std::array<Core::Math::Vec4, 6> planes;
			Core::Math::Vec3				centroid;
			std::array<Core::Math::Vec3, 8> corners;

			void MakeFrustrum(const Camera& cam);
		};

		struct DrawData
		{
			Resources::Mesh* mesh;
			Resources::Texture* albedo;
			Resources::Texture* normalMap;
			Resources::Texture* matMap;

			/* the first is all of them, the second is the ones that are not culled by the frustrum culling of current cam */
			std::vector<Core::Math::Mat4>				matrices;
			std::vector<Core::Math::Mat4>				visibleMatrices;
			std::vector<const ECS::ComponentGameplay*>	gameplays;
			std::vector<const ECS::ComponentGameplay*>	visibleGameplays;

			bool operator==(const ECS::ComponentModel& model);
		};
		
		class DrawDataHandler
		{
			public:
				const ECS::Coordinator*				coordinator	{nullptr};
				Resources::Scene*				currentScene{ nullptr };

				Frustrum							frustrum;

				std::vector<DrawData>	staticDrawData;
				std::vector<DrawData>	dynamicDrawData;
				std::vector<DrawData>	selectedDrawData;
				
				std::array<Core::Math::Vec3, 2>		AABB;
				
				ID3D11DepthStencilView*				depthStencilView;
				ID3D11Buffer*						CamCBuffer;

				ModelDrawer							modelDrawer;
				MapDrawer							mapDrawer;

				const Camera*						currentCam;
				const Gameplay::PlayerData*			player;
				LightsArray*						lights;

			private:
				void PushDrawData(std::vector<DrawData>& drawDatas, const ECS::ComponentModel& model, const Core::Math::Mat4& trs, const ECS::ComponentGameplay& gameplay, bool culled);
				bool Cull(ECS::ComponentModel& model, Core::Math::Mat4& trs);


			public:
				DrawDataHandler();
				~DrawDataHandler();

				void Init(const Game& game);
				void SetScene(Resources::Scene* scene);
				void SetDrawData(const Camera* cam);
				void SetStaticDrawData(const Camera* cam);
				void SetStaticDrawData();
				void Draw(bool drawOccluded = false);
				void Clear();
		};
	}
}

#endif // !__DRAW_DATA_HANDLER_HPP__
