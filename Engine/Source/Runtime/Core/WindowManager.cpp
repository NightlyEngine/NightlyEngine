#include "WindowManager.h"

#include <GLFW/glfw3.h>

namespace Nightly
{
	std::unique_ptr<Window> WindowManager::Create(const WindowProps& props)
	{
		return std::make_unique<Window>(props);
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