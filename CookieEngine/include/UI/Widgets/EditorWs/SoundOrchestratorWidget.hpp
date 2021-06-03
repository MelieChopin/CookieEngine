#ifndef __SOUND_ORCHESTRATOR_W_HPP__
#define __SOUND_ORCHESTRATOR_W_HPP__

#include "UIwidgetBases.hpp"
#include "ResourcesManager.hpp"

namespace Cookie::UIwidget
{
	class SoundOrchestrator final : public WItemBase
	{
	public:
		inline SoundOrchestrator(Resources::ResourcesManager& manager)
			: WItemBase	("Sound orchestrator"), manager(manager)
		{}

		void WindowDisplay() override;

		Resources::ResourcesManager& manager;
	};
}

#endif