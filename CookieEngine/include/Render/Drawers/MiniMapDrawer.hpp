#ifndef __MINI_MAP_DRAWER_HPP__
#define __MINI_MAP_DRAWER_HPP__

#include <vector>

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11Buffer;
struct ID3D11InputLayout;

namespace Cookie
{
	namespace Render
	{
		class DrawDataHandler;
		struct DrawData;

		class MiniMapDrawer
		{
		private:
			ID3D11Buffer* IMatricesBuffer{ nullptr };
			ID3D11Buffer* IArmyBuffer{ nullptr };
			unsigned int  IBufferSize = 0;

		private:
			void AllocateInstance(unsigned int size);

		public:
			MiniMapDrawer();
			~MiniMapDrawer();

			void Draw(const std::vector<DrawData>& toDraw);
		};
	}
}

#endif /* __MINI_MAP_DRAWER_HPP__ */