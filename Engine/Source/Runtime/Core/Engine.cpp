#include "Engine.h"

#include "Core.h"
#include "Log.h"

namespace Nightly
{
	void Engine::Start()
	{
		NL_ASSERT(!m_IsRunning, "Nightly Editor is already running. Multiple instances of the application are not allowed!", ENGINE);

		m_IsRunning = true;

		std::cout << "\n\n" << "Nightly Engine Copyright (C) 2022 Futureblur\n"
		          << "Welcome to Nightly Engine! Enjoy your time and happy coding :)\n\n";

		NL_CORE_INFO("Initializing Nightly Engine...", ENGINE);
	}

	void Engine::Quit()
	{
		NL_CORE_INFO("Quitting Nightly Engine...", ENGINE);
	}
}