#ifndef __SELECTION_DRAWER_HPP__
#define __SELECTION_DRAWER_HPP__

#include <Mat4.hpp>

struct ID3D11Buffer;

#define SELECTION_COLOR {0.0f,1.0f,0.0f,0.5f}

namespace Cookie
{
	namespace Resources
	{
		class Texture;
		class Mesh;
	}

	namespace Render
	{
		class DrawDataHandler;

		class SelectionDrawer
		{
			private:
				std::unique_ptr<Resources::Mesh> arrowMesh{ nullptr };

			public:
				Core::Math::Mat4 viewProj;

				const std::vector<ECS::ComponentModel>*	selectedModels;
				const std::vector<Core::Math::Mat4>*	selectedMatrices;

				std::vector<Core::Math::Mat4> arrowsMatrices;
			public:
				SelectionDrawer();
				~SelectionDrawer();

				void Set(const DrawDataHandler& drawData);
				void Draw(ID3D11Buffer* VCBuffer, ID3D11Buffer* PCBuffer);
				void Clear();
		};
	}
}

#endif // !__SELECTION_DRAWER_HPP__
