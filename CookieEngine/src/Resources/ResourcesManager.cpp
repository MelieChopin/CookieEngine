#include "Resources/Mesh.hpp"
#include "Resources/Shader.hpp"
#include "Render/Renderer.hpp"
#include "Resources/Loader.hpp"
#include "Resources/ResourcesManager.hpp"
#include "Core/Primitives.hpp"


using namespace Cookie::Resources;

ResourcesManager::ResourcesManager()
{
}

ResourcesManager::~ResourcesManager()
{

}

void ResourcesManager::Load(Render::Renderer& _renderer)
{
	if (!HasMesh("Quad"))
		meshes.push_back(Cookie::Core::Primitives::CreateQuad(_renderer));
	if (!HasMesh("Triangle"))
		meshes.push_back(Cookie::Core::Primitives::CreateTriangle(_renderer));

	std::vector<std::string> gltfFiles;
	SearchForGltf(std::string("Assets\\"),gltfFiles);

	Loader loader;

	if (GetDefaultShader() == nullptr)
	{
		std::shared_ptr<Shader> dfltShader = std::make_shared<Shader>(_renderer);
		AddShader(std::move(dfltShader));
	}

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

std::shared_ptr<Mesh> ResourcesManager::AddMesh(std::shared_ptr<Mesh>&& _mesh)
{
	meshes.push_back(_mesh);
	return meshes.back();
}

bool ResourcesManager::HasMesh(std::string _name) const
{
	if (meshes.empty())
		return false;

	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->name == _name)
			return true;
	}

	return false;
}

std::shared_ptr<Mesh> ResourcesManager::GetMesh(std::string _name) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		if (meshes[i]->name == _name)
			return meshes[i];
	}

	return nullptr;
}


std::shared_ptr<Shader> ResourcesManager::AddShader(std::shared_ptr<Shader>&& shader)
{
	shaders.push_back(shader);
	return shaders.back();
}

std::shared_ptr<Shader> ResourcesManager::GetDefaultShader() const
{
	if (!shaders.empty())
		return shaders.front();
	else
		return nullptr;
}

std::shared_ptr<Texture> ResourcesManager::AddTexture(std::shared_ptr<Texture>&& texture)
{
	textures.push_back(texture);
	return textures.back();
}

std::shared_ptr<Texture> ResourcesManager::GetTexture(std::string _name)
{
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (textures[i]->name.find(_name) != std::string::npos)
		{
			return textures[i];
		}
	}

	return nullptr;
}

bool ResourcesManager::HasTexture(std::string _name)
{
	if (textures.empty())
		return false;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (textures[i]->name == _name)
			return true;
	}

	return false;
}