#ifndef	__HIERARCHY_W_HPP__
#define __HIERARCHY_W_HPP__

#include "UIwidgetBases.h"


namespace Cookie::ECS {class EntityHandler;}

namespace Cookie::UIwidget
{
	class Inspector;


	class Hierarchy final : public WindowBase
	{
		Cookie::ECS::EntityHandler& entityHandler;
		Inspector* inspector;

	public:
		Hierarchy(Cookie::ECS::EntityHandler& _entityHandler, Inspector* _inspector)
			: WindowBase("Hierarchy"),
			entityHandler(_entityHandler),
			inspector(_inspector)
		{}

		void WindowDisplay() override;
	};
}

#endif