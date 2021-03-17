#ifndef __SERIALIZATION_HPP__
#define __SERIALIZATION_HPP__


#include <json.hpp>

using json = nlohmann::json;

namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
	}

	namespace Editor
	{
		class Scene;
		namespace Serialization
		{
			namespace Save
			{
				void ToJson(json& js, const Cookie::ECS::EntityHandler& entity);
				void ToJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler component);

				void SaveScene(const char* filepath, const Cookie::Editor::Scene& actScene);
			}

			namespace Load
			{
				void FromJson(json& js, Cookie::ECS::EntityHandler& entity);
				void FromJson(json& js, const Cookie::ECS::EntityHandler& entity, Cookie::ECS::ComponentHandler& component, Cookie::Resources::ResourcesManager resourcesManager);

				void LoadScene(const char* filepath, Cookie::Editor::Scene& newScene, Cookie::Resources::ResourcesManager resourcesManager);
			}
		}
	}
}


#endif // !__SERIALIZATION_HPP__
