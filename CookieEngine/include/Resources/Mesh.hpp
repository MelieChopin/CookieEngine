#ifndef __MESH_HPP__
#define __MESH_HPP__

struct ID3D11Buffer;

#include <string>
#include <assimp/mesh.h>
#include <vector>

namespace Cookie
{
	namespace Render
	{
		class Renderer;
		class RendererRemote;
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
				void InitVBuffer(aiMesh* mesh);
				void InitIBuffer(aiMesh* mesh);
				void InitVBuffer(std::vector<float>& vertices);
				void InitIBuffer(std::vector<unsigned int>& indices);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Mesh(std::string meshName, aiMesh* mesh);
				Mesh(std::string meshName, std::vector<float>& vertices, std::vector<unsigned int>& indices, unsigned int INb);
				~Mesh();

				inline unsigned int GetIndicesNb() { return INb; }
				void Set();
				void Draw();
		};
	}
}

#endif /* __MESH_HPP__ */