#include "Window.h"

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core.h"
#include "Log.h"

namespace Nightly
{
	void Window::Initialize(const WindowProps& props)
	{
		// Initialize GLFW before using it
		NL_ASSERT(glfwInit(), "Failed to initialize GLFW!", ENGINE);

		// Register error callback
		glfwSetErrorCallback(GlfwErrorCallback);

		// Set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		// Create window
		m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
		NL_ASSERT(m_Window, "Failed to create window!", ENGINE);

		glfwMakeContextCurrent(m_Window);

		// Initialize the OpenGL API with GLAD
		NL_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize OpenGL loader!", ENGINE);

		m_IsRunning = true;
	}


	void Window::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::Destroy()
	{
		m_IsRunning = false;
		glfwDestroyWindow(m_Window);
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window) || !m_IsRunning;
	}

	void Window::GlfwErrorCallback(int error, const char* description)
	{
		NL_CORE_ERROR("GLFW: " << description << "\n Error code: " << error, LogSource::ENGINE);
	}
}