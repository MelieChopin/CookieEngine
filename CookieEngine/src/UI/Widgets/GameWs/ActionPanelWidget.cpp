#include "Coordinator.hpp"
#include "ResourcesManager.hpp"
#include "Texture.hpp"
#include "ActionPanelWidget.hpp"

#include "ComponentHandler.hpp"
#include "Prefab.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Resources;
using namespace Cookie::ECS;


void ActionPanel::SafeIcon(const Texture* const & texture, const float size)
{
	static const Texture* const & nullTexture = resources.icons["Assets/UI/IconCanon.tif"].get();

	Image(static_cast<ImTextureID>(texture ? texture->GetResourceView() : nullTexture->GetResourceView()), { size, size });
}


void ActionPanel::WindowDisplay()
{
	TryBeginWindow()
	{
		if (coordinator.selectedEntities.size() == 1)
		{
			const Entity* const& selectedEntity = coordinator.selectedEntities[0];
			const ComponentGameplay& sEntityGameplayComp = coordinator.componentHandler->GetComponentGameplay(selectedEntity->id);

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::PRODUCER)
			{
			
			}
		}
	}
}