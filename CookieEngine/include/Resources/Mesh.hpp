#ifndef __MESH_HPP__
#define __MESH_HPP__

struct ID3D11Buffer;

#include <string>
#include <assimp/mesh.h>

namespace Cookie
{
	namespace Resources
	{
		class Mesh
		{
			private:
				ID3D11Buffer* VBuffer = nullptr;
				ID3D11Buffer* IBuffer = nullptr;

			public:
				std::string name = nullptr;

			private:
				void InitVBuffer(aiMesh* mesh, Render::Renderer& renderer);
				void InitIBuffer(aiMesh* mesh, Render::Renderer& renderer);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Mesh(aiMesh* mesh, Render::Renderer& renderer);
				~Mesh();
		};
	}
}

#endif /* __MESH_HPP__ */