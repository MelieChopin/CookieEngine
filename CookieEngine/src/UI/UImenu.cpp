#include "ResourcesManager.hpp"
#include "UImenu.hpp"

#include <imgui.h>

using namespace ImGui;
using namespace Cookie::UI;
using namespace Cookie::Resources;


void UImenu::DisplayMenus() const
{
	const ImVec2 viewPortSize = GetWindowViewport()->Size;
	
	if (openMenuFlags[(size_t)UImenu::MenusID::MainMenu])
	{
		OpenPopup("Main menu##GAMEMAINMENU");
		openMenuFlags[(size_t)UImenu::MenusID::MainMenu] = false;
	}

	if (openMenuFlags[(size_t)UImenu::MenusID::EndMenu])
	{
		OpenPopup("End menu##GAMEENDMENU");
		openMenuFlags[(size_t)UImenu::MenusID::EndMenu] = false;
	}


	SetNextWindowSize(viewPortSize);
	SetNextWindowViewport(GetWindowViewport()->ID);

	if (BeginPopupModal("Main menu##GAMEMAINMENU", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove))
	{
		menuState[(size_t)UImenu::MenusID::MainMenu] = true;

		Image(static_cast<ImTextureID>(resources.icons["Assets/UI/WallpaperStartGame.tif"]->GetResourceView()), GetContentRegionAvail());


		PushStyleVar(ImGuiStyleVar_FrameRounding, 25.f);

		PushStyleColor(ImGuiCol_Button, {0, 0, 0, 0.85f});
		PushStyleColor(ImGuiCol_ButtonActive, { 0, 0, 0, 1.f });
		PushStyleColor(ImGuiCol_ButtonHovered, { 0, 0, 0, 0.70f });
		

		SetCursorPos({15.f, 20.f});
		
		SetWindowFontScale(5.f);
		TextColored({0, 0, 0, 1}, "Cookie Battle");
		
		SetWindowFontScale(1.5f);

		SetCursorPosX(15.f);
		if (Button("START!", { 150.f, 60.f }))
			CloseCurrentPopup();

		SetWindowFontScale(1.f);

		SetCursorPosX(15.f);
		if (Button("Options", { 100.f, 40.f }));

		SetCursorPosX(15.f);
		if (Button("Quit", { 100.f, 40.f }));


		PopStyleColor(3);
		PopStyleVar();

		EndPopup();
	}
	else menuState[(size_t)UImenu::MenusID::MainMenu] = false;
}