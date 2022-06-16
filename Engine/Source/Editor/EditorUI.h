#pragma once

#include "Core/Core.h"

struct GLFWwindow;

namespace NLE
{
	class EditorWindow;

	class EditorUI
	{
	public:
		EditorUI() = default;
		~EditorUI() = default;

		static void Initialize(GLFWwindow* window);
		static void Draw();
		static void Terminate();

		static void AddEditorWindow(const Ref<EditorWindow>& window);

	private:
		static void ConfigureStyle();

		static inline std::vector<Ref<EditorWindow>> m_WindowRegistry;
	};
}
