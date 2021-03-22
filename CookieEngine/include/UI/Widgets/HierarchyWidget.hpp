#ifndef	__HIERARCHY_W_HPP__
#define __HIERARCHY_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class ResourcesManager; }
namespace Cookie::Resources	{ class Scene; }
namespace Cookie::ECS		{ class Coordinator; }


namespace Cookie::UIwidget
{
	class Inspector;


	class Hierarchy final : public WItemBase
	{
		Cookie::Resources::ResourcesManager&	resources;
		Cookie::Resources::Scene*				scenes;
		Cookie::ECS::Coordinator&				coordinator;

		Inspector* inspector;

	public:
		inline Hierarchy(Cookie::Resources::ResourcesManager& _resources, Cookie::Resources::Scene* _scenes, Cookie::ECS::Coordinator& _coordinator, Inspector* _inspector)
			: WItemBase		("Hierarchy", false),
			  resources     (_resources),
			  scenes		(_scenes),
			  coordinator	(_coordinator),
			  inspector		(_inspector)
		{}

		void WindowDisplay() override;
	};
}

#endif
