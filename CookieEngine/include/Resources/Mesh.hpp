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
				void InitVBuffer(aiMesh* mesh, Render::Renderer& renderer);
				void InitIBuffer(aiMesh* mesh, Render::Renderer& renderer);
				void InitVBuffer(std::vector<float> vertices, Render::Renderer& renderer);
				void InitIBuffer(std::vector<unsigned int> indices, Render::Renderer& renderer);

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Mesh(aiMesh* mesh, Render::Renderer& renderer);
				Mesh(std::vector<float> vertices, std::vector<unsigned int> indices, unsigned int INb, Render::Renderer& renderer);
				~Mesh();

				inline unsigned int GetIndicesNb() { return INb; }
				void Set(Render::RendererRemote& remote);
				void Draw(Render::RendererRemote& remote);
		};
	}
}

#endif /* __MESH_HPP__ */