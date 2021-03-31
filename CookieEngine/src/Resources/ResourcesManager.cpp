#include <d3d11.h>
#include "Resources/Shader/TextureShader.hpp"

#include "Resources/Mesh.hpp"
#include "Render/Renderer.hpp"
#include "Resources/Loader.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Core/Primitives.hpp"
#include "Resources/Prefab.hpp"

using namespace Cookie::Resources;

ResourcesManager::ResourcesManager()
{
	scripts["test.lua"] = std::make_shared<Script>("Assets\\scripts\\test.lua");
	scripts["test2.lua"] = std::make_shared<Script>("Assets\\scripts\\test2.lua");

	InitShaders();
}

ResourcesManager::~ResourcesManager()
{
	meshes.clear();
	shaders.clear();
	textures.clear();
	scripts.clear();
	prefabs.clear();
}


void ResourcesManager::SearchForGltf(const fs::path& path, std::vector<std::string>& gltfFiles)
{

	if (fs::exists(path) && fs::is_directory(path))
	{
		for (const fs::directory_entry& entry : fs::directory_iterator(path))
		{
			const fs::path& filename = entry.path().c_str();
			if (fs::is_directory(entry.status()))
			{
				SearchForGltf(filename,gltfFiles);
			}
		}

		for (const fs::directory_entry& entry : fs::directory_iterator(path))
		{
			const fs::path& filename = entry.path().c_str();
			if (fs::is_regular_file(entry.status()))
			{
				if (filename.string().find(".gltf") != std::string::npos)
					gltfFiles.push_back(filename.string());
			}

		}
	}
}

void ResourcesManager::InitShaders()
{
	shaders["Texture_Shader"] = std::make_shared<TextureShader>("Texture_Shader");
}

void ResourcesManager::Load(Render::Renderer& _renderer)
{
	meshes["Quad"] =  Cookie::Core::Primitives::CreateQuad();
	meshes["Triangle"] = Cookie::Core::Primitives::CreateTriangle();



	std::vector<std::string> gltfFiles;
	SearchForGltf(std::string("Assets\\"),gltfFiles);

	Loader loader;
	
	for (unsigned int i = 0; i < gltfFiles.size(); i++)
	{
		loader.Load(gltfFiles.at(i).c_str(),*this,_renderer);
		printf("%s\n", gltfFiles.at(i).c_str());
	}
}

void ResourcesManager::CreateNewPrefabs(ECS::Entity& entity, ECS::ComponentHandler& component)
{
	Prefab newPrefab;

	newPrefab.name = entity.name;
	newPrefab.nameMesh = component.componentModels[entity.id].mesh->name;
	newPrefab.nameShader = "default";//component.componentModels[entity.id].shader->name;
	newPrefab.nameTexture = component.componentModels[entity.id].texture->name;
	newPrefab.signature = entity.id;
	newPrefab.rotation = component.componentTransforms[entity.id].localTRS.rot;
	newPrefab.scale = component.componentTransforms[entity.id].localTRS.scale;
	newPrefab.filepath = "Assets/Prefabs" + entity.name + ".PAsset";

	entity.namePrefab = entity.name;
}

