#include "Resources/Mesh.hpp"
#include "Resources/ResourcesManager.hpp"

using namespace Cookie::Resources;

ResourcesManager::ResourcesManager()
{

}

ResourcesManager::~ResourcesManager()
{

}

Mesh* ResourcesManager::AddMesh(Mesh* _mesh)
{
	meshes.push_back(_mesh);
	return meshes.back();
}

bool ResourcesManager::HasMesh(std::string _name)
{
	for (unsigned int i = 0; i <= meshes.size(); i++)
	{
		if (meshes[i]->name == _name)
			return true;
	}

	return false;
}

Mesh* ResourcesManager::GetMesh(std::string _name)
{
	for (unsigned int i = 0; i <= meshes.size(); i++)
	{
		if (meshes[i]->name == _name)
			return meshes[i];
	}
}