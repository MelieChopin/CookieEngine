#ifndef __RESOURCES_MANAGER_HPP__
#define __RESOURCES_MANAGER_HPP__

#include <vector>

class Cookie::Resources::Mesh;

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager
		{
			private:
				std::vector<Mesh*> meshes;
				
			public: 
				ResourcesManager();
				~ResourcesManager();

				Mesh* AddMesh(Mesh* mesh);
				Mesh* GetMesh(std::string _name);
				bool HasMesh(std::string _name);
		};
	}
}

#endif /* __RESOURCES_MANAGER_HPP__ */