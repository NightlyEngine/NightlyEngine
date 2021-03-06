#pragma once

#include "PlatformDetection.h"

#ifdef NL_PLATFORM_WINDOWS

#include "Core/Log.h"
#include "Core/PluginManager.h"

namespace NL
{
	// This class allows direct communication with the native WinAPI.
	class NL_API WindowsPlatform
	{
	public:
		// Sets the console color based on the message severity.
		static void SetConsoleColor(const LogSeverity& severity);

		// Sets the console color to white.
		static void ClearConsoleColor();

		// Loads a plugin from a dll file and returns a specification.
		static PluginSpecification LoadPlugin(std::string_view name);

		// Calls FreeLibrary to unload the dll from memory.
		static void UnloadPlugin(const PluginSpecification& plugin);

	private:
		// Contains colors that can be used when printing to the console on Windows.
		struct WinConsoleColors
		{
			static constexpr int White = 15;
			static constexpr int Yellow = 14;
			static constexpr int Red = 12;
		};
	};
}

#endif