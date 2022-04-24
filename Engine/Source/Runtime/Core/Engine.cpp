#include "Engine.h"

#include "Core.h"
#include "Log.h"

namespace Nightly
{
	void Engine::Start()
	{
		NL_ASSERT(!m_IsRunning, "Nightly Editor is already running. Multiple instances of the application are not allowed!", ENGINE);
		m_IsRunning = true;
		NL_CORE_INFO("Initializing Nightly Engine...", ENGINE);
	}

	void Engine::Quit()
	{
		NL_CORE_INFO("Quitting Nightly Engine...", ENGINE);
	}
}