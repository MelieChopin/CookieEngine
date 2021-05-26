#include "Resources/Texture.hpp"
#include "ResourcesManager.hpp"
#include "ToolbarWidget.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UIwidget;


Toolbar::Toolbar(Cookie::Resources::ResourcesManager& _resources, bool& _isPlaying)
	: WindowBase	("##TOOLBAR"),
	  isPlaying		(_isPlaying),

	  icons			{_resources.icons["Assets/EditorUIcons/translate.ico"].get(),
					 _resources.icons["Assets/EditorUIcons/rotate.ico"].get(),
			         _resources.icons["Assets/EditorUIcons/zoom.ico"].get(),
			         _resources.icons["Assets/EditorUIcons/play.ico"].get(),
			         _resources.icons["Assets/EditorUIcons/stop.ico"].get(),
			         _resources.icons["Assets/EditorUIcons/pause.ico"].get(),
			         _resources.icons["Assets/EditorUIcons/frame.ico"].get()}
{}


bool Toolbar::BeginWindow(int windowFlags)
{
	if (!opened) return false;

	contentVisible = ImGui::BeginChild(windowName, {GetWindowContentRegionWidth(), 50}, true, windowFlags);

	return true;
}

void Toolbar::ButtonsDisplay()
{
	PushStyleColor(ImGuiCol_Button,			{ 0.70, 0.70, 0.70, 1 });
	PushStyleColor(ImGuiCol_ButtonActive,	{ 0.35, 0.35, 0.35, 1 });
	PushStyleColor(ImGuiCol_ButtonHovered,	{ 0.90, 0.90, 0.90, 1 });
	
	PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);


	SameLine(15);

	if (ImageButton(icons[(int)ToolbarIcons::Translator]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		currentTrsfTool = TransformTool::Translate;
	} 
	SameLine(15 + (35 * 1));

	if (ImageButton(icons[(int)ToolbarIcons::Rotator]	->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		currentTrsfTool = TransformTool::Rotate;
	}	
	SameLine(15 + (35 * 2));

	if (ImageButton(icons[(int)ToolbarIcons::Scaler]	->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		currentTrsfTool = TransformTool::Scale;
	}		
	SameLine(15 + (35 * 3));


	SameLine(GetWindowWidth() / 2);

	if		(!playing && ImageButton(icons[(int)ToolbarIcons::Play]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		playing = true;
	}
	else if ( playing && ImageButton(icons[(int)ToolbarIcons::Stop]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		playing = false;
	}
	SameLine(GetWindowWidth() / 2 + (35 * 1));


	if (ImageButton(icons[(int)ToolbarIcons::Pause]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		paused ^= 1;
	}
	SameLine(GetWindowWidth() / 2 + (35 * 2));


	if (ImageButton(icons[(int)ToolbarIcons::Frame]->GetResourceView(), { 25, 25 }, { 0, 0 }, { 1, 1 }, 5))
	{
		if (!paused) paused = true;

		frame = true;
	}


	PopStyleVar(1);
	PopStyleColor(3);
}


bool Toolbar::ProcessPlayFlow()
{
	if (playing)
	{
		if (paused)
		{
			if (frame)
			{
				frame = false;
				return true;
			}
			else return false;
		}
		else return true;
	}
	else return false;
}


void Toolbar::WindowDisplay()
{
	TryBeginWindow(ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar)
	{
		ButtonsDisplay();
	}

	ImGui::EndChild();

	
	isPlaying = ProcessPlayFlow();
}