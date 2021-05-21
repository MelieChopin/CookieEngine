#ifndef __PARTICLE_EDITOR_W_HPP__
#define __PARTICLE_EDITOR_W_HPP__

#include "UIwidgetBases.hpp"


namespace Cookie::UIwidget
{
	class ParticleEditor final : public WItemBase
	{
	public:
		inline ParticleEditor()
			: WItemBase("Particle editor")
		{}

		void WindowDisplay() override;
	};
}

#endif