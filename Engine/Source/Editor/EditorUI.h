#pragma once

#include "Core/Core.h"

struct GLFWwindow;
class ImFont;

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

		template <typename T, typename ... Args>
		static void RegisterWindow(Args&& ... args)
		{
			Ref<EditorWindow> window = std::make_shared<T>(std::forward<Args>(args)...);
			m_WindowRegistry.push_back(window);
		}

		static ImFont* RegisterFont(const std::string& path, float size);

		static const ImFont* GetEditorFont()
		{
			return m_EditorFont;
		}

		static const ImFont* GetFiraCodeFont()
		{
			return m_FiraCodeFont;
		}

	private:
		static void ConfigureStyle();

		static inline std::vector<Ref<EditorWindow>> m_WindowRegistry;

		static inline ImFont* m_EditorFont;
		static inline ImFont* m_FiraCodeFont;
	};
}
