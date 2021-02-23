#ifndef	__INSPECTOR_W_HPP__
#define __INSPECTOR_W_HPP__

#include "UIwidgetBases.h"


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

		Cookie::ECS::Entity* selectedEntity = nullptr;

	private:
		void TransformInterface();
		void RigidBodyInterface();
		void ModelCompInterface();
	
	public:
		Inspector(Cookie::ECS::Coordinator& _coordinator)
			: WindowBase		("Inspector"),
			  coordinator		(_coordinator)
		{}

		void WindowDisplay() override;

		inline void SelectEntity(Cookie::ECS::Entity* newSelection)
		{ selectedEntity = newSelection; }
	};
}

#endif