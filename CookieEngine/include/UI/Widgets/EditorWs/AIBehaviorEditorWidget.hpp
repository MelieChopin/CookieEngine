#ifndef	__AIBEHAVIOR_EDITOR_W_HPP__
#define __AIBEHAVIOR_EDITOR_W_HPP__

#include "UIwidgetBases.hpp"
#include "AIBehavior.hpp"

namespace std { using string = basic_string<char, char_traits<char>, allocator<char>>; }
namespace Cookie::Render { class Renderer; }


namespace Cookie
{
	namespace Resources
	{
		class ResourcesManager;
	}

	namespace UIwidget
	{
		class AIBehaviorEditor final : public WItemBase
		{
			Resources::ResourcesManager& resources;
			Gameplay::AIBehavior		 behaviorToEdit;

			Gameplay::AIStep*            selectedStep {nullptr};
			bool			 			 isEditing    {false};

		public:
			inline AIBehaviorEditor(Resources::ResourcesManager& _resources) : WItemBase("AIBehavior editor"), resources(_resources) {}
			void WindowDisplay() override;
		};
	}
}

#endif