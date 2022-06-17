#include "ConsoleOutputWindow.h"

#include <imgui.h>

#include "Core/Log.h"
#include "Core/Utils.h"
#include "EditorUI.h"

using namespace NL;

namespace NLE
{
	void ConsoleOutputWindow::OnDraw()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 4, 4 });
		ImGui::Begin("Console Output", &m_IsOpen);
		ImGui::PopStyleVar();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 8, 4 });

		ImGui::PushItemWidth(100);

		// Log source filter
		if (ImGui::BeginCombo("##LogSourceCombo", m_LogSources[m_SelectedLogSource], ImGuiComboFlags_PopupAlignLeft))
		{
			for (int i = 0; i < IM_ARRAYSIZE(m_LogSources); i++)
			{
				const bool selected = (m_SelectedLogSource == i);
				if (ImGui::Selectable(m_LogSources[i], selected))
				{
					m_SelectedLogSource = i;
				}

				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);

		// Search field
		static char filter[128] = "";
		ImGui::InputTextWithHint("##", "Search ...", filter, IM_ARRAYSIZE(filter), ImGuiInputTextFlags_EnterReturnsTrue);

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(14 / 255.0f, 14 / 255.0f, 14 / 255.0f, 1));
		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(14 / 255.0f, 14 / 255.0f, 14 / 255.0f, 1));
		ImGui::PushFont(const_cast<ImFont*>(EditorUI::GetFiraCodeFont()));

		ImVec2 size(ImGui::GetWindowContentRegionWidth(), ImGui::GetContentRegionAvail().y);
		ImGui::BeginChild("ConsoleContent", size, false, ImGuiWindowFlags_HorizontalScrollbar);

		int i = 0;
		for (const auto& message : Log::GetLogBuffer())
		{
			// Check for Log Source filter
			if (m_SelectedLogSource != 0)
			{
				if (message.GetSource() != m_SelectedLogSource - 1)
				{
					continue;
				}
			}

			// Check if input field is not empty
			if (strcmp(filter, "") != 0)
			{
				auto messageLowercase = Utils::ToLowercase(message.GetContent());

				std::string filterStr = filter;
				auto filterLowercase = Utils::ToLowercase(filterStr);

				if (messageLowercase.find(filterLowercase) == std::string::npos)
				{
					continue;
				}
			}

			ImVec4 color(1, 1, 1, 1);
			if (message.GetSeverity() == LogSeverity::WARNING)
			{
				color = ImVec4(255 / 255.0f, 196 / 255.0f, 81 / 255.0f, 1);
			}
			else if (message.GetSeverity() == LogSeverity::ERROR)
			{
				color = ImVec4(255 / 255.0f, 106 / 255.0f, 106 / 255.0f, 1);
			}
			else if (message.GetSeverity() == LogSeverity::FATAL)
			{
				color = ImVec4(241 / 255.0f, 59 / 255.0f, 59 / 255.0f, 1);
			}

			ImGui::PushStyleColor(ImGuiCol_Text, color);

			ImGui::Indent(10);

			// Console message
			auto text = NL_TEXT("[" << message.GetTimestamp() << "] " << message.GetContent());
			static int selected = -1;
			if (ImGui::Selectable(text.str().c_str(), selected == i, 0))
			{
				selected = i;
			}

			ImGui::Unindent(10);
			ImGui::PopStyleColor();

			// Handle right-clicking
			if (ImGui::BeginPopupContextItem())
			{
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, { 0, 0.5 });
				ImGui::PushFont(const_cast<ImFont*>(EditorUI::GetEditorFont()));
				if (ImGui::Selectable("Copy Message", false, 0, ImVec2(200, ImGui::GetTextLineHeight())))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::Separator();

				if (ImGui::Selectable("Clear Buffer", false, 0, ImVec2(200, ImGui::GetTextLineHeight())))
				{
					Log::ClearLogBuffer();
				}

				ImGui::PopFont();
				ImGui::PopStyleVar();
				ImGui::EndPopup();
			}

			i++;
		}

		ImGui::EndChild();
		ImGui::PopStyleColor(2);
		ImGui::PopFont();

		ImGui::End();
	}
}