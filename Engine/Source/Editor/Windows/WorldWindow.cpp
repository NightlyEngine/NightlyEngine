#include "WorldWindow.h"

#include <imgui.h>

#include "World/WorldManager.h"
#include "World/World.h"
#include "World/Entity.h"

namespace NightlyEditor
{
	void WorldWindow::OnDraw()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImGui::Begin("World", &m_IsOpen);
		ImGui::PopStyleVar();

		for (const auto& entity : Nightly::WorldManager::GetActiveWorld()->GetEntityRegistry())
		{
			const char* name = entity->GetName().c_str();

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_FramePadding |
			                           ImGuiTreeNodeFlags_OpenOnArrow;

			if (entity->HasChildren())
			{
				if (ImGui::TreeNodeEx(name, flags))
				{
					ImGui::TreePop();
				}
			}
			else
			{
				flags |= ImGuiTreeNodeFlags_Leaf;
				if (ImGui::TreeNodeEx(name, flags))
				{
					ImGui::TreePop();
				}
			}
		}

		ImGui::End();
	}
}