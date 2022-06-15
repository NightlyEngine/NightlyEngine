#pragma once

#include "EditorWindow.h"

namespace NightlyEditor
{
	class DockspaceWindow : public EditorWindow
	{
	public:
		DockspaceWindow() = default;
		~DockspaceWindow() override = default;

		void OnDraw() override;
	};
}
