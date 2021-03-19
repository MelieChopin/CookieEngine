#include <GLFW/glfw3.h>
#include "ToolbarWidget.hpp"
#include "FrameBuffer.hpp"
#include "Camera.hpp"
#include "Coordinator.hpp"
#include "ViewportWidget.hpp"

#include <imgui.h>
#include <ImGuizmo.h>

using namespace ImGui;
using namespace Cookie::UIwidget;
using namespace Cookie::ECS;
using namespace Cookie::Core::Math;

#include "Debug.hpp"

void Viewport::WindowDisplay()
{
	TryBeginWindow()
	{
		toolbar->WindowDisplay();
	

		if ((recordedWinSize.x != GetWindowSize().x) || (recordedWinSize.y != GetContentRegionAvail().y))
		{
			recordedWinSize = { {GetWindowSize().x, GetContentRegionAvail().y} };
			(*camera)->SetProj(Core::Math::ToRadians(60.f), recordedWinSize.x, recordedWinSize.y, CAMERA_INITIAL_NEAR, CAMERA_INITIAL_FAR);
		}

		ImGui::Image(static_cast<ImTextureID>(*frameBuffer.GetShaderResource()), GetContentRegionAvail());
		(*camera)->windowOffset = { { ImGui::GetWindowPos().x,  ImGui::GetWindowPos().y } };


		ImGuiIO& io = GetIO();

		if (io.MouseDown[ImGuiMouseButton_Right] && IsWindowHovered())
		{
			if (glfwGetInputMode(window, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
			{
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				io.WantCaptureKeyboard = false;
				(*camera)->Activate();
				(*camera)->ResetPreviousMousePos();
			}
		}
		else if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED && !io.MouseDown[ImGuiMouseButton_Right])
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			io.WantCaptureKeyboard = true;
			(*camera)->Deactivate();
		}
	}

	if (selectedEntity)
		GizmoManipulator();

	ImGui::End();
}

void Viewport::GizmoManipulator()
{
	Transform& trsf = coordinator.componentHandler->GetComponentTransform(selectedEntity->id).localTRS;
	Mat4 trsfTMat = trsf.ToTRS().Transpose();

	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(GetWindowPos().x, GetWindowPos().y, GetWindowSize().x, GetWindowSize().y);
	
	if (ImGuizmo::Manipulate((*camera)->GetView().Transpose().e, (*camera)->GetProj().Transpose().e, ImGuizmo::OPERATION::ROTATE, ImGuizmo::MODE::WORLD, trsfTMat.e))
		ImGuizmo::DecomposeMatrixToComponents(trsfTMat.e, trsf.translation.e, trsf.rotation.e, trsf.scale.e);
}