#ifndef __RESOURCES_MANAGER_HPP__
#define __RESOURCES_MANAGER_HPP__

#include <vector>
#include <memory>
#include <filesystem>
#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"
#include "Resources/Texture.hpp"

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
			private:
				std::vector<std::shared_ptr<Mesh>> meshes;
				std::vector<std::shared_ptr<Shader>> shaders;
				std::vector<std::shared_ptr<Texture>> textures;

			private:
				void SearchForGltf(const fs::path& path, std::vector<std::string>& gltfFiles);
				
			public: 
				ResourcesManager();
				~ResourcesManager();

				void Load(Render::Renderer& _renderer);

				std::shared_ptr<Mesh> AddMesh(std::shared_ptr<Mesh>&& mesh);
				std::shared_ptr<Mesh> GetMesh(std::string _name);
				bool HasMesh(std::string _name);

				std::shared_ptr<Shader> AddShader(std::shared_ptr<Shader>&& shader);
				std::shared_ptr<Shader> GetDefaultShader();

				std::shared_ptr<Texture> AddTexture(std::shared_ptr<Texture>&& texture);
				std::shared_ptr<Texture> GetTexture(std::string _name);
				bool HasTexture(std::string _name);
		};
	}
}

#endif /* __RESOURCES_MANAGER_HPP__ */