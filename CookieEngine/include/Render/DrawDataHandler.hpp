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
			Resources::Mesh*	mesh;
			Resources::Texture* albedo;
			Resources::Texture* normalMap;
			Resources::Texture* matMap;

			/* all the matrices with this model in the world */
			std::vector<Core::Math::Mat4>				matrices;
			/* all the gameplay components with this model in the world */
			std::vector<const ECS::ComponentGameplay*>	gameplays;

			/* the matrices with this model in the world, that were not culled by frustrum culling */
			std::vector<Core::Math::Mat4>				visibleMatrices;
			/* the gameplay components with this model in the world, that were not culled by frustrum culling */
			std::vector<const ECS::ComponentGameplay*>	visibleGameplays;

			bool operator==(const ECS::ComponentModel& model);
		};
		
		class DrawDataHandler
		{
			public:
				/* all the information about the game we need to draw */
				const ECS::Coordinator*				coordinator	{nullptr};
				Resources::Scene*					currentScene{ nullptr };
				const Camera*						currentCam;
				const Gameplay::PlayerData*			player;
				LightsArray*						lights;

				/* the frustrum for occlusion culling */
				Frustrum							frustrum;

				/* draw data for entities that do not move */
				std::vector<DrawData>	staticDrawData;
				/* draw data for entities that do move */
				std::vector<DrawData>	dynamicDrawData;
				/* draw data of selected entities, for them there is a single draw data by entity */
				std::vector<DrawData>	selectedDrawData;
				
				/* AABB of what is visible currently on screen */
				std::array<Core::Math::Vec3, 2>		AABB;
				
				/* info to display the models and map*/
				ID3D11DepthStencilView*				depthStencilView;
				ID3D11Buffer*						CamCBuffer;

				/* the drawers to draw the model ... */
				ModelDrawer							modelDrawer;
				/* ... and to draw the map */
				MapDrawer							mapDrawer;

			private:
				/* */
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
