#pragma once

#include "EngineAPI.h"
#include "Core.h"
#include "ITrackable.h"

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

	// Opaque window object.
	class NL_API Window : public ITrackable<Window>, public std::enable_shared_from_this<Window>
	{
	public:
		explicit Window(const WindowProps& props)
		{
			Initialize(props);
		}

		NL_DEFINE_TRACKABLE(Window);

		// Closes the window.
		void Destroy();

		// Makes the OpenGL context of this window current on the calling thread.
		void MakeContextCurrent();

		// Swaps the front and back buffers of this window.
		void SwapBuffers();

		// Returns true after the window has been created.
		// Returns false if Destroy() was called.
		NL_NODISCARD bool IsRunning() const
		{
			return m_IsRunning;
		}

		// Returns whether the close flag has been set
		// or the window is not running.
		NL_NODISCARD bool ShouldClose() const;

		// Returns a pointer to the GLFWwindow object.
		NL_NODISCARD GLFWwindow* GetNativeWindow() const
		{
			return m_Window;
		}

		NL_NODISCARD int GetWidth() const
		{
			return m_Width;
		}

		NL_NODISCARD int GetHeight() const
		{
			return m_Height;
		}

		NL_NODISCARD float GetAspectRatio() const
		{
			return m_Width / (float) m_Height;
		}

	private:
		void Initialize(const WindowProps& props);

		static void OnWindowResize(GLFWwindow* window, int width, int height);
		static void OnFramebufferResize(GLFWwindow* window, int width, int height);
		static void OnWindowMove(GLFWwindow* window, int x, int y);
		static void OnWindowIconify(GLFWwindow* window, int iconified);
		static void OnWindowMaximize(GLFWwindow* window, int maximized);
		static void OnWindowFocus(GLFWwindow* window, int focused);
		static void OnWindowClose(GLFWwindow* window);

		GLFWwindow* m_Window = nullptr;
		bool m_IsRunning = false;

		int m_Width = 0;
		int m_Height = 0;
	};
}
