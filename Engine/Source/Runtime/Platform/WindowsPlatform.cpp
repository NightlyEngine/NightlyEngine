#include "WindowsPlatform.h"

#ifdef NL_PLATFORM_WINDOWS

#include <windows.h>

#undef ERROR

#include "Core/Plugin.h"
#include "Core/PluginManager.h"

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

	PluginSpecification WindowsPlatform::LoadPlugin(std::string_view name)
	{
		std::stringstream pluginName;
		pluginName << "NightlyPlugin_" << name << ".dll";
		HINSTANCE instance = LoadLibraryA(pluginName.str().c_str());
		if (!instance)
		{
			NL_CORE_FATAL("Plugin library file was not found: " << pluginName.str().c_str(), ENGINE);
			return nullptr;
		}

		using PluginPtr = Plugin* (*)();

		auto plugin = (PluginPtr) GetProcAddress(instance, "GetPluginPtr");
		if (!plugin)
		{
			NL_CORE_FATAL("Failed to load symbols for plugin: " << pluginName.str().c_str(), ENGINE);
			return nullptr;
		}

		PluginSpecification specification;

		specification.pluginPtr = plugin();
		specification.handle = instance;

		return specification;
	}

	void WindowsPlatform::UnloadPlugin(const PluginSpecification& plugin)
	{
		FreeLibrary((HINSTANCE) plugin.handle);
	}
}

#endif
