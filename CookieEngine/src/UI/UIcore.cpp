#include <d3d11.h>

#include "UIcore.h"

#include <imgui.h>
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
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasViewports;
	
	io.ConfigViewportsNoDefaultParent = true;


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

	UpdatePlatformWindows();
	RenderPlatformWindowsDefault();
}

void UIcore::UpdateUI()
{
	BeginFrame();

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

	EndFrame();
}
