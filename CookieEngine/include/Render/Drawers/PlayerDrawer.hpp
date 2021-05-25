#ifndef __PLAYER_DRAWER_HPP__
#define __PLAYER_DRAWER_HPP__

#include <Mat4.hpp>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;

#define PLAYER_INIT_VALID_COLOR {0.0f,1.0f,0.0f}
#define PLAYER_INIT_INVALID_COLOR {1.0f,0.0f,0.0f}

namespace Cookie
{
	namespace Gameplay
	{
		struct PlayerData;
	}

	namespace Resources
	{
		class Texture;
		class Mesh;
	}

	namespace Render
	{


		class PlayerDrawer
		{
		private:
			ID3D11VertexShader* QuadVShader		{ nullptr };
			ID3D11VertexShader* BuildingVShader	{ nullptr };
			ID3D11PixelShader*	PShader			{ nullptr };
			ID3D11Buffer*		VBuffer			{ nullptr };
			ID3D11Buffer*		CBuffer			{ nullptr };

		public:
			struct PlayerDrawInfo
			{
				Core::Math::Vec3 validColor		{ PLAYER_INIT_VALID_COLOR };
				Core::Math::Vec3 invalidColor	{ PLAYER_INIT_INVALID_COLOR };
				bool isValid					= false;
				
				/* for the selection quad, vertex buffer will be changed each time */
				bool isMakingQuad				= false;
				Core::Math::Vec3 quadStart;
				Core::Math::Vec3 quadEnd;

				/* if player is making a building, buildingMesh will not be at nullptr
				 * so it also acts as boolean */
				Resources::Mesh*	buildingMesh		= nullptr;
				Resources::Texture* buildingAlbedoTex	= nullptr;
				Core::Math::Mat4	buildingTRS;
			};


		private:
			void InitShader();

		public:
			PlayerDrawer();
			~PlayerDrawer();

			void Set(const Gameplay::PlayerData& player);
			void Draw();
		};
	}
}

#endif // !__PLAYER_DRAWER_HPP__
