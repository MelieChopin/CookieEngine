#ifndef __RESOURCES_MANAGER_HPP__
#define __RESOURCES_MANAGER_HPP__

#include <vector>
#include <memory>
#include <filesystem>
#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"

namespace fs = ::std::filesystem;

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

			private:
				void SearchForGltf(const fs::path& path, std::vector<std::string>& gltfFiles);
				
			public: 
				ResourcesManager();
				~ResourcesManager();

				void Load(Render::Renderer& _renderer);

				std::shared_ptr<Mesh> AddMesh(std::shared_ptr<Mesh>&& mesh);
				std::shared_ptr<Mesh> GetMesh(std::string _name) const;

				inline const std::vector<std::shared_ptr<Mesh>>&	GetMeshes()	const	{ return meshes; }
				inline const std::vector<std::shared_ptr<Shader>>&	GetShaders() const	{ return shaders;}

				bool HasMesh(std::string _name) const;

				std::shared_ptr<Shader> AddShader(std::shared_ptr<Shader>&& shader);
				std::shared_ptr<Shader> GetDefaultShader() const;
		};
	}
}

#endif /* __RESOURCES_MANAGER_HPP__ */