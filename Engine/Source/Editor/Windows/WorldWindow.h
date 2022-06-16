#pragma once

#include "EditorWindow.h"

namespace NLE
{
	class WorldWindow : public EditorWindow
	{
	public:
		WorldWindow() = default;
		~WorldWindow() override = default;

		void OnDraw() override;
	};
}
