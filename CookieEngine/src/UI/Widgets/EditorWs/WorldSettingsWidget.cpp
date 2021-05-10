#include "Scene.hpp"
#include "ResourcesManager.hpp"
#include "WorldSettingsWidget.hpp"

#include "MapExplorerHelper.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::Resources;


void WorldSettingsWidget::WindowDisplay()
{
	TryBeginWindow()
	{
		// Scale
		{
			Cookie::Core::Math::Vec3& mapScale = scene->map.trs.scale;

			static float xzMapScale[2] = {mapScale.x, mapScale.z};
		
			Text("X/Z Scale:"); SameLine();
			if (DragFloat2("##SCALE", xzMapScale, 0.25))
			{
				if (xzMapScale[0] > 0 && xzMapScale[1] > 0)
				{
					mapScale = { xzMapScale[0], mapScale.y, xzMapScale[1] };
					scene->map.ScaleHasChanged();
				}
			}

			if (xzMapScale[0] <= 0.01 || xzMapScale[1] <= 0.01)
			{
				PushStyleColor(ImGuiCol_Text, {1, 0, 0, 1});
				TextWrapped("Please refrain from inputing very low/negative values.");
				PopStyleColor();
			}
		}


		// Tiles
		{
			Cookie::Core::Math::Vec2& mapTilesNb = scene->map.tilesNb;

			static float xzMapTilesNb[2] = {mapTilesNb.e[0], mapTilesNb.e[1]};

			Text("Tiles in X/Z:"); SameLine();
			if (DragFloat2("##TILES", xzMapTilesNb, 1.f, NULL, NULL, "%.0f"))
			{
				if (xzMapTilesNb[0] > 1 && xzMapTilesNb[1] > 1)
				{
					mapTilesNb = { xzMapTilesNb[0], xzMapTilesNb[1] };
					scene->map.TileNbHasChanged();
				}
			}

			if (xzMapTilesNb[0] <= 1 || xzMapTilesNb[1] <= 1)
			{
				PushStyleColor(ImGuiCol_Text, { 1, 0, 0, 1 });
				TextWrapped("Please refrain from inputing very low/negative values.");
				PopStyleColor();
			}
		}


		// Model
		{
			Text("Map texture:"); SameLine();

			ResourceMapExplorer<Texture>("map texture", "##MAPTEXTSELECT", resources.textures, scene->map.model.texture);
		}
	}

	ImGui::End();
}