#include <assimp/scene.h>
#include <assimp/mesh.h>
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
		InitScene(scene,resources);
	}
	else
	{
		printf("Error parsing '%s': '%s'\n", fileName, importer.GetErrorString());
	}
}

void Loader::InitScene(const aiScene* _scene, ResourcesManager& _resources, Render::Renderer& _renderer)
{
	if (_scene->HasMeshes())
		InitMeshes(_scene->mMeshes,_scene->mNumMeshes,_resources);
}

void Loader::InitMeshes(aiMesh** meshes, unsigned int nMeshes, ResourcesManager& _resources, Render::Renderer& _renderer)
{
	for (unsigned int i = 0; i < nMeshes; i++)
	{
		aiMesh* iMesh = meshes[i];
	}
}