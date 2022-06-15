#pragma once

namespace Nightly
{
	class Window;
}

class ImGui_ImplVulkanH_Window;
class ImDrawData;

namespace NightlyEditor
{
	class EditorUI
	{
	public:
		EditorUI() = default;
		~EditorUI();

		static void Initialize(const std::shared_ptr<Nightly::Window>& editorWindow, ImGui_ImplVulkanH_Window* window);
		static void Update();
		static void Terminate();

	private:
		static void BeginFrame();
		static void EndFrame();

		static void Render();

		static void FrameRender(ImDrawData* drawData);
		static void FramePresent();

		static inline ImGui_ImplVulkanH_Window* m_Window;
	};
}
