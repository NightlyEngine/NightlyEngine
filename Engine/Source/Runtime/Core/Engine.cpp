#include "Engine.h"

#include "Log.h"

namespace Nightly
{
	void Engine::Start()
	{
		if (m_IsRunning)
		{
			NL_CORE_ERROR("Nightly Engine is already running. Multiple instances of the application are not allowed!", LogSource::ENGINE);
			return;
		}

		m_IsRunning = true;

		std::cout << "\n\n" << "Nightly Engine Copyright (C) 2022 Futureblur\n"
		          << "Welcome to Nightly Engine! Enjoy your time and happy coding :)\n\n";

		NL_CORE_INFO("Initializing Nightly Engine...", LogSource::ENGINE);
	}

	void Engine::Quit()
	{
		NL_CORE_INFO("Quitting Nightly Engine...", LogSource::ENGINE);
	}
}