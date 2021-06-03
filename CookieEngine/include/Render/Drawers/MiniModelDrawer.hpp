#ifndef __MINI_MODEL_DRAWER_HPP__
#define __MINI_MODEL_DRAWER_HPP__

#include <vector>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11InputLayout;

#define PLAYER_ARMY_COLOR 0.0f,0.0f,1.0f
#define AI1_ARMY_COLOR 1.0f,0.0f,0.0f

namespace Cookie
{
	namespace Render
	{
		class DrawDataHandler;
		struct DrawData;

		class MiniModelDrawer
		{
		private:
			ID3D11InputLayout* ILayout{ nullptr };
			ID3D11VertexShader* VShader{ nullptr };
			ID3D11PixelShader* PShader{ nullptr };

			ID3D11Buffer* IMatricesBuffer{ nullptr };
			ID3D11Buffer* IArmyBuffer{ nullptr };
			unsigned int  IBufferSize = 0;

		public:
			ID3D11Buffer* CamCBuffer{ nullptr };
			ID3D11Buffer* CBuffer{ nullptr };


		private:
			void InitShader();
			void AllocateInstance(unsigned int size);

		public:
			MiniModelDrawer();
			~MiniModelDrawer();

			void Set();
			void Draw(const std::vector<DrawData>& toDraw);
		};
	}
}

#endif /* __MINI_MODEL_DRAWER_HPP__ */