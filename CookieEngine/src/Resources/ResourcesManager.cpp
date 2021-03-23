#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"
#include "Render/Renderer.hpp"
#include "Resources/Loader.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Core/Primitives.hpp"


using namespace Cookie::Resources;

ResourcesManager::ResourcesManager()
{
	//scripts.push_back(std::make_shared<Script>("Assets\\scripts\\test.lua"));
	//scripts.push_back(std::make_shared<Script>("Assets\\scripts\\test2.lua"));
}

ResourcesManager::~ResourcesManager()
{

}

void ResourcesManager::Load(Render::Renderer& _renderer)
{
	meshes["Quad"] =  Cookie::Core::Primitives::CreateQuad(_renderer);
	meshes["Triangle"] = Cookie::Core::Primitives::CreateTriangle(_renderer);

	std::vector<std::string> gltfFiles;
	SearchForGltf(std::string("Assets\\"),gltfFiles);

	Loader loader;

	shaders["dfltShader"] = std::make_shared<Shader>(_renderer);
	

	for (unsigned int i = 0; i < gltfFiles.size(); i++)
	{
		loader.Load(gltfFiles.at(i).c_str(),*this,_renderer);
		printf("%s\n", gltfFiles.at(i).c_str());
	}
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