#pragma once

#include "EditorWindow.h"

namespace NLE
{
	class ConsoleOutputWindow : public EditorWindow
	{
	public:
		ConsoleOutputWindow() = default;
		~ConsoleOutputWindow() override = default;

		void OnDraw() override;

	private:
		static inline const char* m_LogSources[] = { "All", "Engine", "Editor", "Player", "Plugin" };
		static inline int m_SelectedLogSource = 0;
	};
}
