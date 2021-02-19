#ifndef	__HIERARCHY_W_HPP__
#define __HIERARCHY_W_HPP__

#include "UIwidgetBases.h"


namespace Cookie::ECS {class Coordinator;}

namespace Cookie::UIwidget
{
	class Inspector;


	class Hierarchy final : public WindowBase
	{
		Cookie::ECS::Coordinator& coordinator;
		Inspector* inspector;

	public:
		Hierarchy(Cookie::ECS::Coordinator& _coordinator, Inspector* _inspector)
			: WindowBase	("Hierarchy"),
			  coordinator	(_coordinator),
			  inspector		(_inspector)
		{}

		void WindowDisplay() override;
	};
}

#endif