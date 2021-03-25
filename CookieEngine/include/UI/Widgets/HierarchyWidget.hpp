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
		Cookie::Resources::ResourcesManager&		resources;
		std::shared_ptr<Cookie::Resources::Scene>&	scene;
		Cookie::ECS::Coordinator&					coordinator;

		Cookie::ECS::Entity*& selectedEntity;

	public:
		inline Hierarchy(Cookie::Resources::ResourcesManager& _resources, std::shared_ptr<Cookie::Resources::Scene>& _scene, Cookie::ECS::Coordinator& _coordinator, Cookie::ECS::Entity*& _selectedEntity)
			: WItemBase		("Hierarchy", false),
			  resources     (_resources),
			  scene			(_scene),
			  coordinator	(_coordinator),
			  selectedEntity(_selectedEntity)
		{}

		void WindowDisplay() override;
	};
}

#endif
