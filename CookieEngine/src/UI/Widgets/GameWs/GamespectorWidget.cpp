#include "Coordinator.hpp"
#include "GamespectorWidget.hpp"

#include "ComponentHandler.hpp"
#include "Texture.hpp"
#include "Prefab.hpp"
#include "ResourcesManager.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Resources;
using namespace Cookie::ECS;


void Gamespector::SafeIcon(const Texture* const & texture, const float size)
{
	static const Texture* const & nullTexture = resources.icons["Assets/UI/IconCanon.tif"].get();

	Image(static_cast<ImTextureID>(texture ? texture->GetResourceView() : nullTexture->GetResourceView()), {size, size}); 
}

void Gamespector::LifeBar(const float currentLife, const float maxLife, const float barWidth, const float barHeight)
{
	const float lifeFrac = currentLife / maxLife;

	static float RGBc[3] = {0, 0, 0};
	ColorConvertHSVtoRGB(0.486f * lifeFrac, 1, 1, RGBc[0], RGBc[1], RGBc[2]);

	PushStyleColor(ImGuiCol_PlotHistogram, { RGBc[0], RGBc[1], RGBc[2], 1 });

	std::string lifeBarOverlay = std::to_string((int)currentLife) + std::string("/") + std::to_string((int)maxLife);
	ProgressBar(lifeFrac, { barWidth, barHeight }, lifeBarOverlay.c_str());

	PopStyleColor();
}


void Gamespector::WindowDisplay()
{
	TryBeginWindow()
	{
		if (coordinator.selectedEntities.size() == 1)
		{
			const Entity* const & selectedEntity = coordinator.selectedEntities[0];
			const ComponentGameplay& sEntityGameplayComp = coordinator.componentHandler->GetComponentGameplay(selectedEntity->id);
			
			Dummy({GetContentRegionAvail().x / 4.f, GetContentRegionAvail().y});
			
			SameLine();
			BeginGroup();

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::LIVE)
			{
				const float regionHeight = GetContentRegionAvail().y;

				SafeIcon(coordinator.componentHandler->GetComponentModel(selectedEntity->id).icon, regionHeight - 19.f);
				
				LifeBar(sEntityGameplayComp.componentLive.lifeCurrent, sEntityGameplayComp.componentLive.lifeMax, regionHeight - 19.f, 15.f);
			}
			else
				SafeIcon(coordinator.componentHandler->GetComponentModel(selectedEntity->id).icon, GetContentRegionAvail().y);

			EndGroup();
			SameLine();

			BeginGroup();

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::PRODUCER)
			{

				if (sEntityGameplayComp.componentProducer.queueOfUnits.size() > 0)
				{
					const std::vector<Prefab*>& unitsQueue = sEntityGameplayComp.componentProducer.queueOfUnits;
					
					SafeIcon(unitsQueue[0]->model.icon, 50.f);

					SameLine();
					BeginGroup();

					Dummy({0.f, 31.f});
					
					PushStyleColor(ImGuiCol_PlotHistogram, { 0, 1, 1, 1 });
					ProgressBar(sEntityGameplayComp.componentProducer.currentCountdown / unitsQueue[0]->gameplay.cost.timeToProduce, { 160.f, 15.f }, std::to_string((int)sEntityGameplayComp.componentProducer.currentCountdown).c_str());
					PopStyleColor();

					EndGroup();

					if (unitsQueue.size() > 1)
					{
						for (size_t i = 1; i < unitsQueue.size(); i++)
						{
							SafeIcon(unitsQueue[i]->model.icon, 40.f);

							SameLine();
						}

						NewLine();
					}
					else Dummy({0, 40.f});
				}
				else
				{
					Dummy({ 50.f, 50.f });

					SameLine();
					BeginGroup();

					Dummy({ 0.f, 31.f });

					PushStyleColor(ImGuiCol_PlotHistogram, { 0.5, 0.5, 0.5, 1 });
					ProgressBar(1.f, { GetContentRegionAvail().x / 3.f, 15.f }, "No production");
					PopStyleColor();

					EndGroup();

					Dummy({ 0, 40.f });
				}

				Separator();
			}

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::RESOURCE)
			{
				if (sEntityGameplayComp.componentResource.isPrimary)
				{
					TextColored({ 1.f, 0.816f, 0.31f, 1.f }, "Remaining wheat: %.2f units", sEntityGameplayComp.componentResource.resourceReserve);
				}
				else
				{
					TextColored({ 0.235f, 0.16f, 0.16f, 255.f }, "Remaining chocolate: %.2f units", sEntityGameplayComp.componentResource.resourceReserve);
				}
			}

			EndGroup();
		}
		else if (coordinator.selectedEntities.size() > 1)
		{
			float size = (int)GetContentRegionAvail().y * 0.75f;
			int iconsPerLines = GetContentRegionAvail().x / (size + 8);

			while ((int)((coordinator.selectedEntities.size() / iconsPerLines) + 1) * (size + 4) > GetContentRegionAvail().y)
			{
				size /= 2.f;
				iconsPerLines = GetContentRegionAvail().x / (size + 8);
			}


			int placedTemp = 0;

			for (const Entity* const & e : coordinator.selectedEntities)
			{
				SafeIcon(coordinator.componentHandler->GetComponentModel(e->id).icon, size);


				const ComponentGameplay& eGC = coordinator.componentHandler->GetComponentGameplay(e->id);

				if (eGC.signatureGameplay && IsItemHovered())
				{
					BeginTooltip();
					
					if (eGC.signatureGameplay & CGP_SIGNATURE::LIVE)
						LifeBar(eGC.componentLive.lifeCurrent, eGC.componentLive.lifeMax, 50.f, 15.f);
					
					if ((eGC.signatureGameplay & CGP_SIGNATURE::PRODUCER) && (eGC.componentProducer.queueOfUnits.size() > 0))
					{
						SafeIcon(eGC.componentProducer.queueOfUnits[0]->model.icon, 50.f);

						SameLine();
						BeginGroup();

						Dummy({ 0.f, 31.f });

						PushStyleColor(ImGuiCol_PlotHistogram, { 0, 1, 1, 1 });
						ProgressBar(eGC.componentProducer.currentCountdown / eGC.componentProducer.queueOfUnits[0]->gameplay.cost.timeToProduce, { 160.f, 15.f }, std::to_string((int)eGC.componentProducer.currentCountdown).c_str());
						PopStyleColor();

						EndGroup();
					}
					
					EndTooltip();
				}

				placedTemp++;

				(placedTemp % iconsPerLines) ? SameLine() : NewLine();
			}
		}
	}

	ImGui::End();
}