#pragma once

namespace NightlyEditor
{
	class EditorWindow
	{
	public:
		EditorWindow() = default;
		virtual ~EditorWindow() = default;

		virtual void OnDraw() = 0;

	protected:
		bool m_IsOpen = true;
	};
}
