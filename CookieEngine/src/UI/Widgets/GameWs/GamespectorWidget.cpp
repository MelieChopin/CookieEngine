#include "Coordinator.hpp"
#include "GamespectorWidget.hpp"

#include "ComponentHandler.hpp"
#include "Texture.hpp"
#include "Prefab.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Resources;
using namespace Cookie::ECS;


void Gamespector::WindowDisplay()
{
	TryBeginWindow()
	{
		if (coordinator.selectedEntities.size() == 1)
		{
			const Entity* const & selectedEntity = coordinator.selectedEntities[0];

			const Texture* const & assignedIcon = coordinator.componentHandler->GetComponentModel(selectedEntity->id).icon;
			const ImTextureID usedSEicon = static_cast<ImTextureID>(assignedIcon != nullptr ? assignedIcon->GetResourceView() : nullTexture->GetResourceView());
			
			const ComponentGameplay& sEntityGameplayComp = coordinator.componentHandler->GetComponentGameplay(selectedEntity->id);
			
			
			BeginGroup();

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::LIVE)
			{
				const float regionHeight = GetContentRegionAvail().y;

				Image(usedSEicon, {regionHeight - 19.f, regionHeight - 19.f});
				

				const float lifeFrac = sEntityGameplayComp.componentLive.lifeCurrent / sEntityGameplayComp.componentLive.lifeMax;

				Cookie::Core::Math::Vec3 RGBc;
				ColorConvertHSVtoRGB(0.486f * lifeFrac, 1, 1, RGBc.r, RGBc.g, RGBc.b);

				PushStyleColor(ImGuiCol_PlotHistogram, ColorConvertFloat4ToU32({ RGBc.r, RGBc.g, RGBc.b, 1 }));

				std::string lifeBarOverlay = std::to_string((int)sEntityGameplayComp.componentLive.lifeCurrent) + std::string("/") + std::to_string((int)sEntityGameplayComp.componentLive.lifeMax);
				ProgressBar(lifeFrac, {regionHeight - 19.f, 15.f}, lifeBarOverlay.c_str());
				
				PopStyleColor();
			}
			else
				Image(usedSEicon, { GetContentRegionAvail().y, GetContentRegionAvail().y });

			EndGroup();
			SameLine();

			BeginGroup();

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::PRODUCER)
			{
				
			}

			EndGroup();
		}
	}

	ImGui::End();
}