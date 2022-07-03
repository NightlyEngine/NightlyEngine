#include "WindowManager.h"

#include <GLFW/glfw3.h>

#include "Core.h"
#include "Log.h"
#include "Window.h"

namespace NL
{
	void WindowManager::Initialize()
	{
		NL_ASSERT(glfwInit(), "Failed to initialize GLFW!", ENGINE);
		m_IsInitialized = true;

		// Register error callback
		glfwSetErrorCallback(GlfwErrorCallback);
	}

	Ref<Window> WindowManager::Create(const WindowProps& props)
	{
		auto window = MakeRef<Window>(props);
		m_WindowRegistry.push_back(window);
		return window;
	}

	void WindowManager::PollEvents()
	{
		glfwPollEvents();
	}

	void WindowManager::Terminate()
	{
		glfwTerminate();
	}

	void WindowManager::GlfwErrorCallback(int error, const char* description)
	{
		NL_CORE_ERROR("GLFW: " << description << "\n Error code: " << error, ENGINE);
	}
}