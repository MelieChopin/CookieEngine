#ifndef __DRAW_DATA_HANDLER_HPP__
#define __DRAW_DATA_HANDLER_HPP__

#include <Mat4.hpp>
#include <array>
#include <vector>

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

		struct Frustrum
		{
			std::array<Core::Math::Vec4, 6> planes;
			Core::Math::Vec3				centroid;
			std::array<Core::Math::Vec3, 8> corners;
			std::array<Core::Math::Vec3, 2> AABB;
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

				struct MapInfo
				{
					std::unique_ptr<Resources::Mesh> mapMesh;
					Resources::Texture* albedo{ nullptr };
					Resources::Texture* normal{ nullptr };
					Resources::Texture* metallicRoughness{ nullptr };

					Core::Math::Mat4 TRS;
				};

				MapInfo mapInfo;

				ID3D11DepthStencilView*				depthStencilView;
				ID3D11Buffer*						CamCBuffer;

			private:
				void InitCBuffer();
				void MakeFrustrum(const Camera& cam);

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
