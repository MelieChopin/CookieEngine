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


bool ActionPanel::SafeIconButton(const Texture* const & texture, const float size)
{
	static const Texture* const & nullTexture = resources.icons["Assets/EditorUIcons/Log.ico"].get();

	return ImageButton(static_cast<ImTextureID>(texture ? texture->GetResourceView() : nullTexture->GetResourceView()), { size, size });
}


void ActionPanel::WindowDisplay()
{
	TryBeginWindow()
	{
		if (coordinator.selectedEntities.size() == 1)
		{
			const Entity* const & selectedEntity = coordinator.selectedEntities[0];
			ComponentGameplay& sEntityGameplayComp = coordinator.componentHandler->GetComponentGameplay(selectedEntity->id);

			const float iconSize = (GetContentRegionAvail().y / 4.f);

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::PRODUCER)
			{
				for (size_t i = 0; i < sEntityGameplayComp.componentProducer.possibleUnits.size(); i++)
				{
					if (SafeIconButton(sEntityGameplayComp.componentProducer.possibleUnits[i]->model.icon, iconSize))
						sEntityGameplayComp.componentProducer.AddUnitToQueue(i);

					(i % 4) ? SameLine() : NewLine();
				}

			}
		}
	}

	ImGui::End();
}