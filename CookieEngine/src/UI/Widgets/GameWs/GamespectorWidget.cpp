#include "Coordinator.hpp"
#include "GamespectorWidget.hpp"

#include "ComponentHandler.hpp"

#include "Prefab.hpp"

#include <imgui.h>

using namespace Cookie::UIwidget;
using namespace Cookie::ECS;
using namespace ImGui;


void Gamespector::WindowDisplay()
{
	TryBeginWindow()
	{
		if (coordinator.selectedEntities.size() == 1)
		{
			const Entity* const & selectedEntity = coordinator.selectedEntities[0];
			const ComponentGameplay& sEntityGameplayComp = coordinator.componentHandler->GetComponentGameplay(selectedEntity->id);

			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::LIVE)
			{
				const unsigned int rectNums = GetContentRegionAvail().x / 5;
				const unsigned int litRects = (rectNums * sEntityGameplayComp.componentLive.lifeCurrent) / sEntityGameplayComp.componentLive.lifeMax;

				Cookie::Core::Math::Vec3 RGBc;
				ColorConvertHSVtoRGB((175.f * litRects) / rectNums, 100, 100, RGBc.r, RGBc.g, RGBc.b);

				const ImU32 color = ColorConvertFloat4ToU32({RGBc.r, RGBc.g, RGBc.b, 1});

				for (unsigned int i = 0; i < rectNums; i++)
				{
					if (i <= litRects)
						GetWindowDrawList()->AddRectFilled({ 0, 0 }, { 5, 10 }, color);
					else
						GetWindowDrawList()->AddRect({ 0, 0 }, { 5, 10 }, color);

					SameLine();
				}

				NewLine();
			}

			
			if (sEntityGameplayComp.signatureGameplay & CGP_SIGNATURE::PRODUCER)
			{

			}
		}
	}

	ImGui::End();
}