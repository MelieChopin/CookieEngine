#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <string>
#include "Render/Renderer.hpp"
#include <assimp/postprocess.h>
#include "Resources/ResourcesManager.hpp"
#include "Resources/Loader.hpp"

using namespace Cookie::Resources;

Loader::Loader()
{
}

Loader::~Loader()
{
}

void Loader::Load(const char* fileName, ResourcesManager& resources, Render::Renderer& _renderer)
{
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices);

	if (scene)
	{
		InitScene(fileName,scene,resources,_renderer);
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", fileName, importer.GetErrorString());
	}
}

void Loader::InitScene(const char* fileName, const aiScene* _scene, ResourcesManager& _resources, Render::Renderer& _renderer)
{
	if (_scene->HasMeshes())
		InitMeshes(_scene->mMeshes,_scene->mNumMeshes,_resources, _renderer);
	if (_scene->HasMaterials())
		InitTextures(std::string(fileName).substr(0, std::string(fileName).find_last_of("\\/")).c_str(),_scene->mMaterials, _scene->mNumMaterials, _resources, _renderer);
}

void Loader::InitMeshes(aiMesh** meshes, unsigned int nMeshes, ResourcesManager& _resources, Render::Renderer& _renderer)
{
	for (unsigned int i = 0; i < nMeshes; i++)
	{
		aiMesh* iMesh = meshes[i];
		if (!_resources.HasMesh(iMesh->mName.C_Str()))
			_resources.AddMesh(std::move(std::make_shared<Mesh>(iMesh, _renderer)));
	}

}

void Loader::InitTextures(const char* pathName, aiMaterial** materials, unsigned int nMatNb, ResourcesManager& _resources, Render::Renderer& _renderer)
{
	for (unsigned int i = 0; i < nMatNb; i++)
	{
		aiMaterial* iMat = materials[i];
		aiString path;
		if (aiReturn_SUCCESS == aiGetMaterialTexture(iMat, aiTextureType_DIFFUSE, 0, &path))
		{
			std::string fullpath = (std::string(pathName) + '/' + std::string(path.C_Str())).c_str();

			if (!_resources.HasTexture(fullpath.c_str()))
				_resources.AddTexture(std::move(std::make_shared<Texture>(_renderer, fullpath.c_str())));

			printf("%s\n", (std::string(pathName) + std::string(path.C_Str())).c_str());
		}
	}
}