#ifndef	__HIERARCHY_W_HPP__
#define __HIERARCHY_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class ResourcesManager; }
namespace Cookie::Editor	{ class Scene; }
namespace Cookie::ECS		{ class Coordinator; }


namespace Cookie::UIwidget
{
	class Inspector;


	class Hierarchy final : public WItemBase
	{
		Cookie::Resources::ResourcesManager&	resources;
		Cookie::Editor::Scene*					scenes;
		Cookie::ECS::Coordinator&				coordinator;

		Inspector* inspector;

	public:
		inline Hierarchy(Cookie::Resources::ResourcesManager& _resources, Cookie::Editor::Scene* _scenes, Cookie::ECS::Coordinator& _coordinator, Inspector* _inspector)
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
