#include <d3d11.h>

#include "UI/UIcore.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_dx11.h>

#include "Renderer.hpp"


using namespace ImGui;
using namespace Cookie::Core;


UIcore::UIcore(GLFWwindow* _window, const Cookie::Render::Renderer& _renderer)
		: window (_window)
{
	IMGUI_CHECKVERSION();
	CreateContext();
	io = &ImGui::GetIO();

	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io->BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	io->BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
	
	io->ConfigViewportsNoDefaultParent = true;


	StyleColorsDark();

	GetStyle().Colors[ImGuiCol_DockingEmptyBg]	= {0, 0, 0, 0};

	ImGui_ImplGlfw_InitNoAPI(_window, true);
	ImGui_ImplDX11_Init(_renderer.GetDevice(), _renderer.remote.context);
	ImGui_ImplDX11_CreateDeviceObjects();
}

void UIcore::Terminate()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}


void UIcore::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();
}

void UIcore::EndFrame()
{	
	ImGui::Render();	
	ImGui_ImplDX11_RenderDrawData(GetDrawData());


	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		UpdatePlatformWindows();
		RenderPlatformWindowsDefault();
	}


}

void UIcore::UpdateUI()
{
	BeginFrame();

	if (mouseCaptured)
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX); // Disable ImGui mouse handling


	DockSpaceOverViewport(GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	// Main Window space
	{
		BeginMainMenuBar();

		for (int i = 0; i < menus.size(); i++)
		{
			if (BeginMenu(menus[i]))
			{
				for (UIwidget::ItemBase* cI : UItems[i])
				{
					cI->ItemDisplay();
				}

				ImGui::EndMenu();
			}

			for (UIwidget::WindowBase* cW : UIndows[i])
			{
				cW->WindowDisplay();
			}
		}

		EndMainMenuBar();


		for (UIwidget::WindowBase* cOW : UIndows[menus.size()])
		{
			cOW->WindowDisplay();
		}
	}

	// Mouse capture (Mouse right click to enable, escape key to disable)
	{
		if (ImGui::IsKeyPressed(GLFW_KEY_ESCAPE) && mouseCaptured)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			mouseCaptured = false;
		}

		if (!ImGui::GetIO().WantCaptureMouse && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			mouseCaptured = true;
		}
	}

	EndFrame();
}
