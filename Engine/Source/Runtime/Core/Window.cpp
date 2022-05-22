#include "Window.h"

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Log.h"
#include "WindowManager.h"

#include "Event/EventSystem.h"
#include "Event/WindowEvents.h"
#include "Core/Event/InputSystem.h"

namespace Nightly
{
	void Window::Initialize(const WindowProps& props)
	{
		// Only continue if WindowManager has already been initialized
		NL_ASSERT(WindowManager::IsInitialized(), "Initialize WindowManager first before creating the first window!", ENGINE);

		// Set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Create window
		m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
		NL_ASSERT(m_Window, "Failed to create window!", ENGINE);
		NL_CORE_INFO("Created new window: " << props.title, ENGINE);

		MakeContextCurrent();

		// Initialize the OpenGL API with GLAD
		NL_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize OpenGL loader!", ENGINE);

		m_Width = props.width;
		m_Height = props.height;

		m_IsRunning = true;

		// Register callbacks
		glfwSetWindowSizeCallback(m_Window, OnWindowResize);
		glfwSetFramebufferSizeCallback(m_Window, OnFramebufferResize);
		glfwSetWindowPosCallback(m_Window, OnWindowMove);
		glfwSetWindowIconifyCallback(m_Window, OnWindowIconify);
		glfwSetWindowMaximizeCallback(m_Window, OnWindowMaximize);
		glfwSetWindowFocusCallback(m_Window, OnWindowFocus);
		glfwSetWindowCloseCallback(m_Window, OnWindowClose);
		glfwSetKeyCallback(m_Window, InputSystem::KeyCallback);
		glfwSetMouseButtonCallback(m_Window, InputSystem::MouseButtonCallback);
	}

	void Window::MakeContextCurrent()
	{
		glfwMakeContextCurrent(m_Window);
	}

	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::Destroy()
	{
		if (!m_IsRunning) return;
		
		m_IsRunning = false;
		glfwDestroyWindow(m_Window);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window) || !m_IsRunning;
	}

	void Window::OnWindowResize(GLFWwindow* window, int width, int height)
	{
		Events::OnWindowResize event(window, width, height);
		EventSystem::Dispatch(event);
	}

	void Window::OnFramebufferResize(GLFWwindow* window, int width, int height)
	{
		Events::OnFramebufferResize event(window, width, height);
		EventSystem::Dispatch(event);
	}

	void Window::OnWindowMove(GLFWwindow* window, int x, int y)
	{
		Events::OnWindowMove event(window, x, y);
		EventSystem::Dispatch(event);
	}

	void Window::OnWindowIconify(GLFWwindow* window, int iconified)
	{
		Events::OnWindowIconify event(window, iconified);
		EventSystem::Dispatch(event);
	}

	void Window::OnWindowMaximize(GLFWwindow* window, int maximized)
	{
		Events::OnWindowMaximize event(window, maximized);
		EventSystem::Dispatch(event);
	}

	void Window::OnWindowFocus(GLFWwindow* window, int focused)
	{
		Events::OnWindowFocus event(window, focused);
		EventSystem::Dispatch(event);
	}

	void Window::OnWindowClose(GLFWwindow* window)
	{
		Events::OnWindowClose event(window);
		EventSystem::Dispatch(event);
	}
}