#pragma once

#include "EditorWindow.h"

namespace NLE
{
	class DockspaceWindow : public EditorWindow
	{
	public:
		DockspaceWindow() = default;
		~DockspaceWindow() override = default;

		void OnDraw() override;
	};
}
