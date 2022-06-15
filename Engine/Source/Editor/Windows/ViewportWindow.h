#pragma once

#include "EditorWindow.h"

namespace NightlyEditor
{
	class ViewportWindow : public EditorWindow
	{
	public:
		ViewportWindow() = default;
		~ViewportWindow() override = default;

		void OnDraw() override;
	};
}
