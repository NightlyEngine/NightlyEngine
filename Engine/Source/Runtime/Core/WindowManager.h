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

		// Creates a new window object using the given properties.
		static std::unique_ptr<Window> Create(const WindowProps& props);

		// Processes events that are in the event queue.
		static void PollEvents();

		// Destroys all windows and terminates the GLFW library.
		static void Terminate();
	};
}
