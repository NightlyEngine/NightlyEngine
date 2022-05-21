#pragma once

#include "EngineAPI.h"
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
	class NL_API Window : public ITrackable<Window>
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

	private:
		void Initialize(const WindowProps& props);

		GLFWwindow* m_Window = nullptr;
		bool m_IsRunning = false;
	};
}
