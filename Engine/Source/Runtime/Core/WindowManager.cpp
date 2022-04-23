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
}