#include "WindowsPlatform.h"

#ifdef NL_PLATFORM_WINDOWS

#include <windows.h>

#undef ERROR

#include "Core/Plugin.h"

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

	Plugin* WindowsPlatform::LoadPlugin(std::string_view name)
	{
		HINSTANCE instance = LoadLibrary(name + R".dll");
		if (!instance)
		{
			NL_CORE_FATAL("Plugin library file was not found: " << name << ".dll", ENGINE);
			return nullptr;
		}

		void* plugin = GetProcAddress(instance, "GetPluginPtr");
		if (!plugin)
		{
			NL_CORE_FATAL("Failed to load symbols for plugin: " << name << ".dll", ENGINE);
			return nullptr;
		}

		using PluginPtr = Plugin* (*)();
		auto fun = reinterpret_cast<PluginPtr>(plugin);

		return fun();
	}
}

#endif
