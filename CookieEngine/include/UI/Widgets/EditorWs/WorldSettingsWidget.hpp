#ifndef __WORLD_SETTINGS_W_HPP__
#define __WORLD_SETTINGS_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class Scene; class ResourceManager; }

namespace Cookie::Render	{ struct LightsArray; }


namespace Cookie::UIwidget
{
	class WorldSettingsWidget final : public WItemBase
	{
		std::shared_ptr<Resources::Scene>& scene;
		Cookie::Resources::ResourcesManager& resources;

		Cookie::Render::LightsArray& lights;

	public:
		inline WorldSettingsWidget(std::shared_ptr<Resources::Scene>& _scene, Cookie::Resources::ResourcesManager& _resources, Cookie::Render::LightsArray& _lights)
			: WItemBase	("World settings", false),
			  scene		(_scene),
			  resources (_resources),
			  lights	(_lights)
		{}

		void WindowDisplay() override;
	};
}

#endif // !__WORLD_SETTING_W_HPP__
