#include <d3d11.h>

#include "UIeditor.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_dx11.h>

#include <ImGuizmo.h>

#include "Renderer.hpp"


using namespace ImGui;
using namespace Cookie::UI;


UIeditor::UIeditor(const Cookie::Render::Renderer& _renderer)
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

	ImGui_ImplGlfw_InitNoAPI(_renderer.window.window, true);
	ImGui_ImplDX11_Init(_renderer.GetDevice(), _renderer.remote.context);
	ImGui_ImplDX11_CreateDeviceObjects();
}

void UIeditor::Terminate()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	DestroyContext();
}


void UIeditor::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();
	ImGuizmo::BeginFrame();
}

void UIeditor::EndFrame()
{	
	ImGui::Render();	
	ImGui_ImplDX11_RenderDrawData(GetDrawData());

	UpdatePlatformWindows();
	RenderPlatformWindowsDefault();
}

void UIeditor::UpdateUI()
{
	BeginFrame();

	DockSpaceOverViewport(GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	// Main Window space
	{
		BeginMainMenuBar();

		for (int i = 0; i < menus.size(); i++)
		{
			if (ImGui::BeginMenu(menus[i]))
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