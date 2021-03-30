#ifndef	__INSPECTOR_W_HPP__
#define __INSPECTOR_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources  { class ResourcesManager; }
namespace Cookie::ECS		 { class Coordinator; class Entity; }
namespace Cookie::Core::Math { union Vec2; }
namespace Cookie			 { struct FocusEntity; }


namespace Cookie::UIwidget
{
	class Inspector final : public WItemBase
	{
		FocusEntity&		selectedEntity;
	
		Cookie::Resources::ResourcesManager& resources;
		Cookie::ECS::Coordinator& coordinator;

		Physics::PhysicsSimulator& physSim;

	private:
		void EntityInspection();

		void TransformInterface();
		void ModelInterface();
		void PhysicsInterface();
		void ScriptInterface();
		void MapInterface();

		void SceneInspection();

	public:
		inline Inspector(FocusEntity& _selectedEntity, Cookie::Resources::ResourcesManager& _resources, Cookie::ECS::Coordinator& _coordinator, Physics::PhysicsSimulator& _physSim)
			: WItemBase			("Inspector", false),
			  selectedEntity	(_selectedEntity),
			  resources			(_resources),
			  coordinator		(_coordinator),
			  physSim			(_physSim)
		{}

		void WindowDisplay() override;
	};
}

#endif
