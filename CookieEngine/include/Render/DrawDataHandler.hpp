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

	namespace ECS
	{
		class ComponentModel;
	}

	namespace Resources
	{
		class Texture;
		class Mesh;
	}

	namespace Render
	{
		class Camera;

		class Frustrum
		{
		public :
			std::array<Core::Math::Vec4, 6> planes;
			Core::Math::Vec3				centroid;
			std::array<Core::Math::Vec3, 8> corners;
			std::array<Core::Math::Vec3, 2> AABB;

			void MakeFrustrum(const Camera& cam);
		};
		
		class DrawDataHandler
		{
			public:
				ID3D11Buffer* CBuffer{nullptr};
				Frustrum frustrum;
				std::vector<ECS::ComponentModel>	models;
				std::vector<Core::Math::Mat4>		matrices;
				std::array<Core::Math::Vec3, 2>		AABB;
				const Camera*						currentCam;

				ID3D11DepthStencilView*				depthStencilView;
				ID3D11Buffer*						CamCBuffer;
				

				MapDrawer							mapDrawer;

			private:
				void InitCBuffer();
				//void MakeFrustrum(const Camera& cam);

			public:
				DrawDataHandler();
				~DrawDataHandler();

				void SetDrawData(const Camera* cam, const Game& game);
				void Draw(int _i = 0);
				void Clear();
		};
	}
}

#endif // !__DRAW_DATA_HANDLER_HPP__
