#pragma once

#include "EngineAPI.h"

namespace Nightly
{
	class Window;
	struct WindowProps;

	// Wraps around the GLFW library and handles window states.
	class NL_API WindowManager
	{
	public:
		WindowManager() = default;
		~WindowManager() = default;

		// Initializes the GLFW library and sets error callbacks.
		static void Initialize();

		// Returns whether the GLFW library has already been initialized.
		static bool IsInitialized()
		{
			return m_IsInitialized;
		}

		// Creates a new window object using the given properties.
		static std::shared_ptr<Window> Create(const WindowProps& props);

		// FIXME:   Returns the firstly created window for now.
		//          In the future, this function will return the focused window.
		static inline std::shared_ptr<Window> GetCurrentWindow()
		{
			return !m_WindowRegistry.empty() ? m_WindowRegistry[0] : nullptr;
		}

		// Processes events that are in the event queue.
		static void PollEvents();

		// Destroys all windows and terminates the GLFW library.
		static void Terminate();

	private:
		static void GlfwErrorCallback(int error, const char* description);

		static inline bool m_IsInitialized = false;

		static inline std::vector<std::shared_ptr<Window>> m_WindowRegistry;
	};
}
