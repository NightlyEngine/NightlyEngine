#pragma once

#ifdef NL_PLATFORM_WINDOWS

#include "Core/Log.h"

namespace Nightly
{
	// This class allows direct communication with the native WinAPI.
	class WindowsPlatform
	{
	public:
		// Sets the console color based on the message severity.
		static void SetConsoleColor(const LogSeverity& severity);

		// Sets the console color to white.
		static void ClearConsoleColor();

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