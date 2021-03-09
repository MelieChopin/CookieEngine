#ifndef	__INSPECTOR_W_HPP__
#define __INSPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class ResourcesManager; }
namespace Cookie::ECS		{ class Coordinator; class Entity; }
namespace Cookie::Editor	{ class Scene; }


namespace Cookie::UIwidget
{
	class Inspector final : public WItemBase
	{
		Cookie::Resources::ResourcesManager& resources;
		Cookie::ECS::Coordinator& coordinator;

		Cookie::ECS::Entity*	selectedEntity	= nullptr;
		Cookie::Editor::Scene*	selectedScene	= nullptr;

	private:
		void EntityInspection();

		void TransformInterface();
		void RigidBodyInterface();
		void ModelCompInterface();
	
		
		void SceneInspection();

	public:
		Inspector(Cookie::Resources::ResourcesManager& _resources, Cookie::ECS::Coordinator& _coordinator)
			: WItemBase			("Inspector", false),
			  resources			(_resources),
			  coordinator		(_coordinator)
		{}

		void WindowDisplay() override;


		inline void SelectEntity(Cookie::ECS::Entity* newSelection)
		{ selectedEntity = newSelection; selectedScene = nullptr; }

		inline void SelectScene(Cookie::Editor::Scene* newSelection)
		{ selectedScene = newSelection; selectedEntity = nullptr; }
	};
}

#endif