#ifndef __RESOURCES_MANAGER_HPP__
#define __RESOURCES_MANAGER_HPP__

#include <map>
#include <memory>
#include <filesystem>
#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Texture.hpp"
#include "Resources/Script.hpp"
#include "Resources/Scene.hpp"


namespace fs = std::filesystem;

namespace Cookie
{
	namespace ECS
	{
		class ComponentHandler;
	}

	namespace Resources
	{
		class ResourcesManager
		{
			public:
				std::unordered_map<std::string, std::shared_ptr<Mesh>>		meshes;
				std::unordered_map<std::string, std::shared_ptr<Shader>>	shaders;
				std::unordered_map<std::string, std::shared_ptr<Texture>>	textures;
				std::unordered_map<std::string, std::shared_ptr<Script>>	scripts;

			private:
				void SearchForGltf(const fs::path& path, std::vector<std::string>& gltfFiles);
				
			public: 
				ResourcesManager();
				~ResourcesManager();


				void Load(Render::Renderer& _renderer);

				void UpdateScriptsContent()
				{
					for (std::unordered_map<std::string, std::shared_ptr<Script>>::iterator scrIt = scripts.begin(); scrIt != scripts.end(); scrIt++)
						if (!(scrIt->second->isUpToDate()))
							scrIt->second->UpdateContent();
				}
		};
	}
}

#endif /* __RESOURCES_MANAGER_HPP__ */