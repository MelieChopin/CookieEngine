#ifndef __SERIALIZATION_HPP__
#define __SERIALIZATION_HPP__


#include <json.hpp>

using json = nlohmann::json;

namespace Cookie
{
	class Game;

	namespace ECS
	{
		class Coordinator;
	}

	namespace reactphysics3d
	{
		class PhysicsCommon;
	}

	namespace Resources
	{
		class ResourcesManager;
		class Scene;
		class Prefab;
	}

	namespace Resources
	{
		namespace Serialization
		{
			namespace Save
			{
				void ToJson(json& js, const Cookie::ECS::EntityHandler& entity);
				void ToJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler& component, 
					Cookie::Resources::ResourcesManager& resourcesManager);

				void SaveScene(Cookie::Resources::Scene& actScene, Cookie::Resources::ResourcesManager& resourcesManager);
				void SavePrefab(const std::shared_ptr<Prefab>& prefab);

				void SaveAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager);
			}

			namespace Load
			{
				void FromJson(json& js, Cookie::ECS::EntityHandler& entity);
				void FromJson(json& js, const Cookie::ECS::EntityHandler& entity, 
					Cookie::ECS::ComponentHandler& component, Cookie::Resources::ResourcesManager& resourcesManager);

				std::shared_ptr<Scene> LoadScene(const char* filepath, Game& game);
				void LoadAllPrefabs(Cookie::Resources::ResourcesManager& resourcesManager);
			}
		}
	}
}


#endif // !__SERIALIZATION_HPP__
