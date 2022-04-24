#pragma once

#include "EngineAPI.h"
#include "Window.h"

namespace Nightly
{
	// Wraps around the GLFW library and handles window states.
	class NL_API WindowManager
	{
	public:
		WindowManager() = default;
		~WindowManager() = default;

		static void Initialize();

		static bool IsInitialized()
		{
			return m_IsInitialized;
		}

		// Creates a new window object using the given properties.
		static std::unique_ptr<Window> Create(const WindowProps& props);

		// Processes events that are in the event queue.
		static void PollEvents();

		// Destroys all windows and terminates the GLFW library.
		static void Terminate();

	private:
		static void GlfwErrorCallback(int error, const char* description);

		static inline bool m_IsInitialized = false;
	};
}
