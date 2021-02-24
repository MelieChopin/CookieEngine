#ifndef	__HIERARCHY_W_HPP__
#define __HIERARCHY_W_HPP__

#include "UIwidgetBases.h"
#include "Resources/ResourcesManager.hpp"

namespace Cookie::ECS {class Coordinator;}


namespace Cookie::UIwidget
{
	class Inspector;


	class Hierarchy final : public WindowBase
	{
		Cookie::ECS::Coordinator& coordinator;
		Cookie::Resources::ResourcesManager& resources;
		Inspector* inspector;

	public:
		Hierarchy(Cookie::ECS::Coordinator& _coordinator, Inspector* _inspector, Cookie::Resources::ResourcesManager& _resources)
			:	WindowBase	("Hierarchy"),
				coordinator	(_coordinator),
				resources     (_resources),
				inspector(_inspector)
		{}

		void WindowDisplay() override;
	};
}

#endif