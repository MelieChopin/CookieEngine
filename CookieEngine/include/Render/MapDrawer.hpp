#ifndef __MAP_DRAWER_HPP__
#define __MAP_DRAWER_HPP__

#include <Mat4.hpp>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

namespace Cookie
{
	namespace Resources
	{
		class Texture;
		class Mesh;
		class Map;
	}

	namespace Render
	{


		class MapDrawer
		{
			private:
				ID3D11VertexShader* VShader{ nullptr };
				ID3D11Buffer*		CBuffer{ nullptr };

			public:
				struct MapInfo
				{
					std::unique_ptr<Resources::Mesh>	mapMesh;
					Resources::Texture*					albedoTex = nullptr;
					Resources::Texture*					normalTex = nullptr;
					Resources::Texture*					matTex = nullptr;

					Core::Math::Mat4					model;
					Core::Math::Vec2					tileSize;
				};

				MapInfo mapInfo;

			private:
				void InitShader();

			public:
				MapDrawer();
				~MapDrawer();

				void Set(const Resources::Map& map);
				void Draw();
		};
	}
}

#endif // !__MAP_DRAWER_HPP__
