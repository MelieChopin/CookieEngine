#ifndef __WORLD_SETTINGS_W_HPP__
#define __WORLD_SETTINGS_W_HPP__

#include "UIwidgetBases.hpp"

namespace Cookie::Resources { class Scene; class ResourceManager; }


namespace Cookie::UIwidget
{
	class WorldSettingsWidget final : public WItemBase
	{
		std::shared_ptr<Resources::Scene>& scene;
		Cookie::Resources::ResourcesManager& resources;

	public:
		inline WorldSettingsWidget(std::shared_ptr<Resources::Scene>& _scene, Cookie::Resources::ResourcesManager& _resources)
			: WItemBase	("World settings", false),
			  scene		(_scene),
			  resources (_resources)
		{}

		void WindowDisplay() override;
	};
}

#endif // !__WORLD_SETTING_W_HPP__
