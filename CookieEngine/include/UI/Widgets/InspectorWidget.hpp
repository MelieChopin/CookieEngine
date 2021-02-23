#ifndef	__INSPECTOR_W_HPP__
#define __INSPECTOR_W_HPP__


#include "UIwidgetBases.h"
#include "Resources/ResourcesManager.hpp"

namespace Cookie::ECS
{
	class Coordinator;
	class Entity;
}


namespace Cookie::UIwidget
{
	class Inspector final : public WindowBase
	{
		Cookie::ECS::Coordinator& coordinator;
		Cookie::Resources::ResourcesManager& resources;

		Cookie::ECS::Entity* selectedEntity = nullptr;

	private:
		void TransformInterface();
		void RigidBodyInterface();
		void ModelCompInterface();
	
	public:
		Inspector(Cookie::ECS::Coordinator& _coordinator, Cookie::Resources::ResourcesManager& _resources)
			: WindowBase		("Inspector"),
			  coordinator		(_coordinator),
			  resources			(_resources)
		{}

		void WindowDisplay() override;

		inline void SelectEntity(Cookie::ECS::Entity* newSelection)
		{ selectedEntity = newSelection; }
	};
}

#endif