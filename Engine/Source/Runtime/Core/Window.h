#pragma once

#include "EngineAPI.h"

struct GLFWwindow;

namespace Nightly
{
	// Specifies the properties of a window during creation.
	struct WindowProps
	{
		std::string title = "Untitled Window";
		int width;
		int height;
	};

	class NL_API Window
	{
	public:
		explicit Window(const WindowProps& props)
		{
			Initialize(props);
		}

		void Destroy();
		void SwapBuffers();

		// Returns true after the window has been created.
		// Returns false if Destroy() was called.
		bool IsRunning() const
		{
			return m_IsRunning;
		}

		// Returns whether the close flag has been set
		// or the window is not running.
		bool ShouldClose() const;

		// Returns a pointer to the GLFWwindow object.
		GLFWwindow* GetNativeWindow() const
		{
			return m_Window;
		}

	private:
		void Initialize(const WindowProps& props);
		static void GlfwErrorCallback(int error, const char* description);

		GLFWwindow* m_Window = nullptr;
		bool m_IsRunning = false;
	};
}
