#include "EditorUI.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "Platform/PlatformDetection.h"

#include "Windows/EditorWindow.h"
#include "Windows/DockspaceWindow.h"
#include "Windows/WorldWindow.h"
#include "Windows/ViewportWindow.h"

namespace NLE
{
	void EditorUI::Initialize(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		io.ConfigWindowsResizeFromEdges = true;

		#ifdef NL_PLATFORM_OSX
		io.ConfigMacOSXBehaviors = true;
		#endif

		io.Fonts->AddFontFromFileTTF("../../Source/ThirdParty/imgui-cmake/misc/fonts/Roboto-Medium.ttf", 16.0f);

		ConfigureStyle();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 150");

		auto dockspaceWindow = std::make_shared<DockspaceWindow>();
		auto worldWindow = std::make_shared<WorldWindow>();
		auto viewportWindow = std::make_shared<ViewportWindow>();

		AddEditorWindow(dockspaceWindow);
		AddEditorWindow(worldWindow);
		AddEditorWindow(viewportWindow);
	}

	void EditorUI::Draw()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		for (const auto& window : m_WindowRegistry)
		{
			window->OnDraw();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorUI::Terminate()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void EditorUI::AddEditorWindow(const Ref<EditorWindow>& window)
	{
		m_WindowRegistry.push_back(window);
	}

	void EditorUI::ConfigureStyle()
	{
		ImGui::StyleColorsDark();

		ImColor bg(34, 34, 34);
		ImColor frameBg(24, 24, 24);
		ImColor black(14, 14, 14);
		ImColor itemFg(110, 110, 110);
		ImColor buttonFg(70, 70, 70);
		ImColor hoverAdd(15, 15, 15);
		ImColor headerBg(50, 50, 50);
		ImColor primary(92, 60, 255);
		ImColor transparent(0, 0, 0, 0);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0;
		style.FrameRounding = 3;
		style.GrabRounding = 2;
		style.WindowTitleAlign = { 0.5f, 0.5f };
		style.WindowMenuButtonPosition = ImGuiDir_None;

		style.Colors[ImGuiCol_WindowBg] = bg;
		style.Colors[ImGuiCol_ScrollbarBg] = bg;
		style.Colors[ImGuiCol_TitleBg] = black;
		style.Colors[ImGuiCol_TitleBgActive] = black;
		style.Colors[ImGuiCol_FrameBg] = frameBg;
		style.Colors[ImGuiCol_FrameBgHovered] = frameBg + hoverAdd;
		style.Colors[ImGuiCol_FrameBgActive] = frameBg + hoverAdd + hoverAdd;
		style.Colors[ImGuiCol_Border] = ImColor(70, 70, 70);
		style.Colors[ImGuiCol_SliderGrab] = itemFg;
		style.Colors[ImGuiCol_SliderGrabActive] = itemFg + hoverAdd;
		style.Colors[ImGuiCol_Tab] = frameBg;
		style.Colors[ImGuiCol_TabHovered] = headerBg + hoverAdd;
		style.Colors[ImGuiCol_TabActive] = headerBg;
		style.Colors[ImGuiCol_TabUnfocusedActive] = bg;
		style.Colors[ImGuiCol_Header] = headerBg;
		style.Colors[ImGuiCol_HeaderHovered] = headerBg + hoverAdd;
		style.Colors[ImGuiCol_HeaderActive] = headerBg + hoverAdd + hoverAdd;
		style.Colors[ImGuiCol_CheckMark] = itemFg + ImColor(30, 30, 30);
		style.Colors[ImGuiCol_Button] = buttonFg;
		style.Colors[ImGuiCol_ButtonHovered] = buttonFg + hoverAdd;
		style.Colors[ImGuiCol_ButtonActive] = buttonFg + hoverAdd + hoverAdd;
		style.Colors[ImGuiCol_DockingPreview] = primary;
		style.Colors[ImGuiCol_DockingEmptyBg] = black;
		style.Colors[ImGuiCol_ResizeGrip] = transparent;
		style.Colors[ImGuiCol_ResizeGripHovered] = transparent;
		style.Colors[ImGuiCol_ResizeGripActive] = transparent;
		style.Colors[ImGuiCol_Separator] = black;
		style.Colors[ImGuiCol_SeparatorHovered] = primary;
		style.Colors[ImGuiCol_SeparatorActive] = primary;
	}
}