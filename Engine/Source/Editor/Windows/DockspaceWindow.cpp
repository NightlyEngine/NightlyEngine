#include "DockspaceWindow.h"

#include <imgui.h>

namespace NightlyEditor
{
	void DockspaceWindow::OnDraw()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
		                               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("Dockspace", nullptr, windowFlags);
		ImGui::PopStyleVar(3);

		ImGuiID id = ImGui::GetID("Dockspace");
		ImGui::DockSpace(id, ImVec2(0.0f, 0.0f));

		ImGui::End();
	}
}