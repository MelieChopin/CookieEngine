#include "Resources/Texture.hpp"
#include "Renderer.hpp"
#include "ToolbarWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


Toolbar::Toolbar(Cookie::Render::Renderer& _renderer)
	: WindowBase	("##TOOLBAR")
{
	icons[(int)ToolbarIcons::Translator]	= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/translate.ico");
	icons[(int)ToolbarIcons::Rotator]		= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/rotate.ico");
	icons[(int)ToolbarIcons::Scaler]		= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/zoom.ico");
	icons[(int)ToolbarIcons::Quader]		= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/quad.ico");
	icons[(int)ToolbarIcons::Play]			= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/play.ico");
	icons[(int)ToolbarIcons::Stop]			= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/stop.ico");
	icons[(int)ToolbarIcons::Pause]			= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/pause.ico");
	icons[(int)ToolbarIcons::Frame]			= std::make_unique<Cookie::Resources::Texture>("Assets/EditorUIcons/frame.ico");
}

Toolbar::~Toolbar()
{
	for (int i = 0; i < (int)ToolbarIcons::ICOS_NUM; i++)
	{
		icons[i].reset();
	}
}

bool Toolbar::BeginWindow(int windowFlags)
{
	if (!opened) return false;

	contentVisible = ImGui::BeginChild(windowName, {GetWindowContentRegionWidth(), 50}, true, windowFlags);

	return true;
}

void Toolbar::WindowDisplay()
{
	ImGuiStyle prev = GetStyle();


	ImVec4(&ImColors)[55] = GetStyle().Colors;
	ImColors[ImGuiCol_Button]			= { 0.70, 0.70, 0.70, 1 };
	ImColors[ImGuiCol_ButtonActive]		= { 0.35, 0.35, 0.35, 1 };
	ImColors[ImGuiCol_ButtonHovered]	= { 0.90, 0.90, 0.90, 1 };


	TryBeginWindow(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)
	{
		SameLine(15);

		GetStyle().FrameRounding = 5.f;
		if (ImageButton(icons[(int)ToolbarIcons::Translator]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{ currentTrsfTool = TransformTool::Translate; } SameLine(15 +(35*1));

		if (ImageButton(icons[(int)ToolbarIcons::Rotator]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{ currentTrsfTool = TransformTool::Rotate; }	SameLine(15 +(35*2));

		if (ImageButton(icons[(int)ToolbarIcons::Scaler]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{ currentTrsfTool = TransformTool::Scale; }		SameLine(15 +(35*3));

		if (ImageButton(icons[(int)ToolbarIcons::Quader]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{ currentTrsfTool = TransformTool::Quad; } 
		

		SameLine(GetWindowWidth()/2);

		if		(!playing && ImageButton(icons[(int)ToolbarIcons::Play]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{
			playing = true; 
			/*TBD*/
		}
		else if (playing && ImageButton(icons[(int)ToolbarIcons::Stop]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{
			playing = false; 
			/*TBD*/
		}	
		
		SameLine(GetWindowWidth()/2 +(35*1));


		if (ImageButton(icons[(int)ToolbarIcons::Pause]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{/*TBD*/}	SameLine(GetWindowWidth()/2 +(35*2));

		if (ImageButton(icons[(int)ToolbarIcons::Frame]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
		{/*TBD*/}
	}

	ImGui::EndChild();


	GetStyle() = prev;
}