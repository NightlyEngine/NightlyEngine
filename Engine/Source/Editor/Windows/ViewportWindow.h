#pragma once

#include "EditorWindow.h"

namespace NLE
{
	class ViewportWindow : public EditorWindow
	{
	public:
		ViewportWindow() = default;
		~ViewportWindow() override = default;

		void OnDraw() override;
	};
}
