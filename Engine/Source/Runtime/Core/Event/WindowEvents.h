#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"
#include "Event.h"

struct GLFWwindow;

namespace Nightly::Events
{
	struct NL_API WindowEvent : public Event
	{
		explicit WindowEvent(GLFWwindow* window)
				: m_Window(window)
		{
		}

		~WindowEvent() override = default;

		NL_NODISCARD virtual GLFWwindow* GetNativeWindow()
		{
			return m_Window;
		}

	private:
		GLFWwindow* m_Window = nullptr;
	};

	class NL_API OnWindowResize : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnWindowResize);

		OnWindowResize(GLFWwindow* window, int width, int height)
				: WindowEvent(window), m_Width(width), m_Height(height)
		{
		}

		~OnWindowResize() override = default;

		NL_NODISCARD int GetWidth() const
		{
			return m_Width;
		}

		NL_NODISCARD int GetHeight() const
		{
			return m_Height;
		}

	private:
		int m_Width = 0;
		int m_Height = 0;
	};

	class NL_API OnFramebufferResize : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnFramebufferResize);

		OnFramebufferResize(GLFWwindow* window, int width, int height)
				: WindowEvent(window), m_Width(width), m_Height(height)
		{
		}

		~OnFramebufferResize() override = default;

		NL_NODISCARD int GetWidth() const
		{
			return m_Width;
		}

		NL_NODISCARD int GetHeight() const
		{
			return m_Height;
		}

	private:
		int m_Width = 0;
		int m_Height = 0;
	};

	class NL_API OnWindowMove : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnWindowMove);

		OnWindowMove(GLFWwindow* window, int x, int y)
				: WindowEvent(window), m_X(x), m_Y(y)
		{
		}

		~OnWindowMove() override = default;

		NL_NODISCARD int GetX() const
		{
			return m_X;
		}

		NL_NODISCARD int GetY() const
		{
			return m_Y;
		}

	private:
		int m_X = 0;
		int m_Y = 0;
	};

	class NL_API OnWindowIconify : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnWindowIconify);

		OnWindowIconify(GLFWwindow* window, bool iconified)
				: WindowEvent(window), m_Iconified(iconified)
		{
		}

		~OnWindowIconify() override = default;

		NL_NODISCARD bool IsIconified() const
		{
			return m_Iconified;
		}

	private:
		bool m_Iconified = false;
	};

	class NL_API OnWindowMaximize : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnWindowMaximize);

		OnWindowMaximize(GLFWwindow* window, int maximized)
				: WindowEvent(window), m_Maximized(maximized)
		{
		}

		~OnWindowMaximize() override = default;

		NL_NODISCARD bool IsMaximized() const
		{
			return m_Maximized;
		}

	private:
		bool m_Maximized = false;
	};

	class NL_API OnWindowFocus : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnWindowFocus);

		OnWindowFocus(GLFWwindow* window, bool focused)
				: WindowEvent(window), m_Focused(focused)
		{
		}

		~OnWindowFocus() override = default;

		NL_NODISCARD bool IsFocused() const
		{
			return m_Focused;
		}

	private:
		bool m_Focused = false;
	};

	class NL_API OnWindowClose : public WindowEvent
	{
	public:
		NL_DEFINE_EVENT(OnWindowClose);

		explicit OnWindowClose(GLFWwindow* window) : WindowEvent(window)
		{
		}

		~OnWindowClose() override = default;
	};
}
