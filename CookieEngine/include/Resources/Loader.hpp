#ifndef __LOADER_HPP__
#define __LOADER_HPP__

#include <assimp/Importer.hpp>

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;

		class Loader
		{
			private:
				Assimp::Importer importer;

			public:
				/* CONSTRUCTORS/DESTRUCTORS */
				Loader();
				~Loader();

				void Load(const char* fileName, ResourcesManager& _resources, Render::Renderer& renderer);
				void InitScene(const aiScene* _scene, ResourcesManager& _resources, Render::Renderer& renderer);
				void InitMeshes(aiMesh** meshes, unsigned int nMeshes, ResourcesManager& _resources, Render::Renderer& renderer);
		};
	}
}

#endif /* __LOADER_HPP__ */
