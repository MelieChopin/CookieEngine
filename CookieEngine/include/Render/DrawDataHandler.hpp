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
			std::vector<Core::Math::Mat4>		matrices;
			std::vector<Core::Math::Mat4>		visibleMatrices;
			std::vector<int>					id;
			std::vector<int>					visibleId;

			bool operator==(const ECS::ComponentModel& model);
		};
		
		class DrawDataHandler
		{
			public:
				const ECS::Coordinator*				coordinator	{nullptr};
				Frustrum							frustrum;

				std::vector<DrawData>	staticDrawData;
				std::vector<DrawData>	dynamicDrawData;
				
				std::array<Core::Math::Vec3, 2>		AABB;
				
				ID3D11DepthStencilView*				depthStencilView;
				ID3D11Buffer*						CamCBuffer;

				ModelDrawer							modelDrawer;
				MapDrawer							mapDrawer;

				const Camera*						currentCam;
				const Gameplay::PlayerData*			player;
				LightsArray*						lights;

				std::vector<ECS::ComponentModel>	selectedModels;
				std::vector<Core::Math::Mat4>		selectedMatrices;
				std::vector<ECS::ComponentGameplay> selectedGameplays;

			private:
				void PushDrawData(std::vector<DrawData>& drawDatas, const ECS::ComponentModel& model, const Core::Math::Mat4& trs, int entityId, bool culled);

			public:
				DrawDataHandler();
				~DrawDataHandler();

				void Init(const Game& game);
				void SetScene(Resources::Scene& scene);
				void SetDrawData(const Camera* cam);
				void Draw(bool drawOccluded = false);
				void Clear();
		};
	}
}

#endif // !__DRAW_DATA_HANDLER_HPP__
