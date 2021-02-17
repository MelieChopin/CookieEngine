#ifndef __MESH_HPP__
#define __MESH_HPP__

struct ID3D11Buffer;

#include <string>
#include <assimp/mesh.h>

namespace Cookie
{
	namespace Render
	{
		class Renderer;
	}

	namespace Resources
	{
		class Mesh
		{
			private:
				ID3D11Buffer* VBuffer	= nullptr;
				ID3D11Buffer* IBuffer	= nullptr;
				unsigned int  INb		= 0;

			public:
				std::string name;

			private:
				void InitVBuffer(aiMesh* mesh, Render::Renderer& renderer);
				void InitIBuffer(aiMesh* mesh, Render::Renderer& renderer);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Mesh(aiMesh* mesh, Render::Renderer& renderer);
				~Mesh();

				inline unsigned int GetIndicesNb() { return INb; }
		};
	}
}

#endif /* __MESH_HPP__ */