#ifndef	__INSPECTOR_W_HPP__
#define __INSPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources  { class ResourcesManager; }
namespace Cookie::ECS		 { class Coordinator; class Entity; }
namespace Cookie::Core::Math { union Vec2; }


namespace Cookie::UIwidget
{
	class Inspector final : public WItemBase
	{
		Cookie::Resources::ResourcesManager& resources;
		Cookie::ECS::Coordinator& coordinator;

		Cookie::ECS::Entity*&		selectedEntity;
	
	private:
		void EntityInspection();

		void TransformInterface();
		void RigidBodyInterface();
		void ModelInterface();
		void ScriptInterface();
		void MapInterface();

		void SceneInspection();

	public:
		inline Inspector(Cookie::Resources::ResourcesManager& _resources, Cookie::ECS::Coordinator& _coordinator, Cookie::ECS::Entity*& _selectedEntity)
			: WItemBase			("Inspector", false),
			  resources			(_resources),
			  coordinator		(_coordinator),
			  selectedEntity	(_selectedEntity)
		{}

		void WindowDisplay() override;
	};
}

#endif
