#include "FrameBuffer.hpp"
#include "Camera.hpp"
#include "Map.hpp"
#include "MinimapWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


void Minimap::WindowDisplay()
{
	PushStyleVar(ImGuiStyleVar_WindowPadding, {0, 0});

	TryBeginWindow()
	{
		Image(static_cast<ImTextureID>(minimapBuffer.shaderResource), GetContentRegionAvail());

		if (IsItemHovered() && GetIO().MouseDown[0])
		{
			const ImVec2& mousePos = GetIO().MousePos;

			camera->pos.x = ((mousePos.x - GetWindowPos().x) / GetWindowSize().x) * map.trs.scale.x - (map.trs.scale.x * 0.5);
			camera->pos.z = ((mousePos.y - GetWindowPos().y) / GetWindowSize().y) * map.trs.scale.z - (map.trs.scale.z * 0.5);
		}
	}

	ImGui::End();

	PopStyleVar();
}
