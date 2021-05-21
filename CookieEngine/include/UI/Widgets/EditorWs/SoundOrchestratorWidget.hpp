#ifndef __SOUND_ORCHESTRATOR_W_HPP__
#define __SOUND_ORCHESTRATOR_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	class SoundOrchestrator final : public WItemBase
	{
	public:
		inline SoundOrchestrator()
			: WItemBase	("Sound orchestrator")
		{}

		void WindowDisplay() override;
	};
}

#endif