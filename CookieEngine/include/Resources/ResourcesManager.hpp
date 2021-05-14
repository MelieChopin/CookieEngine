#ifndef __RESOURCES_MANAGER_HPP__
#define __RESOURCES_MANAGER_HPP__


#include <unordered_map>
#include <memory>
#include <filesystem>
#include <fmod.hpp>
#include <d3d11.h>

namespace fs = std::filesystem;

namespace Cookie
{
	namespace ECS
	{
		class ComponentHandler;
		class Entity;
	}

	namespace Render
	{
		class Renderer;
	}

	namespace Resources
	{

		class Mesh;
		class Shader;
		class Texture;
		class Script;
		class Prefab;

		class ResourcesManager
		{
			public:
				std::unordered_map<std::string, std::unique_ptr<Mesh>>			meshes;
				std::unordered_map<std::string, std::unique_ptr<Texture>>		textures;
				std::unordered_map<std::string, std::shared_ptr<Script>>		scripts;
				std::unordered_map<std::string, std::shared_ptr<Prefab>>		prefabs;
				
			private:
				void SearchForAssets(const fs::path& path, std::vector<std::string>& gltfFiles);

				void InitPrimitives();
				
			public: 
				ResourcesManager();
				~ResourcesManager();

				void Load(Render::Renderer& _renderer);

				void UpdateScriptsContent();

				void CreateNewPrefabs(ECS::Entity& entity, ECS::ComponentHandler& component);
		};
	}
}

#endif /* __RESOURCES_MANAGER_HPP__ */
