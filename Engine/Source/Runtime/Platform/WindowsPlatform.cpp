#include "WindowsPlatform.h"

#ifdef NL_PLATFORM_WINDOWS
#include <windows.h>

namespace Nightly
{
	void WindowsPlatform::SetConsoleColor(const LogSeverity& severity)
	{
		int color;

		switch (severity)
		{
			case LogSeverity::INFO:
				color = WinConsoleColors::White;
				break;

			case LogSeverity::WARNING:
				color = WinConsoleColors::Yellow;
				break;

			case LogSeverity::ERROR:
				color = WinConsoleColors::Red;
				break;

			case LogSeverity::FATAL:
				color = WinConsoleColors::Red;
				break;

			default:
				break;
		}

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	void WindowsPlatform::ClearConsoleColor()
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, WinConsoleColors::White);
	}

}

#endif
