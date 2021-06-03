#ifndef __MODEL_DRAWER_HPP__
#define __MODEL_DRAWER_HPP__

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

		class ModelDrawer
		{
		private:
			ID3D11Buffer*		IBuffer{ nullptr };
			unsigned int		IBufferSize = 0;

		private:
			void AllocateInstance(unsigned int size);

		public:
			ModelDrawer();
			~ModelDrawer();

			void Draw(const std::vector<DrawData>& toDraw, bool drawOccluded);
		};
	}
}

#endif /* __MODEL_DRAWER_HPP__ */