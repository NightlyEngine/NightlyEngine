#include "PluginManager.h"

#include <array>

#include "Log.h"
#include "Plugin.h"

#if defined(NL_PLATFORM_WINDOWS)
#include "Platform/WindowsPlatform.h"
#else

#include <dlfcn.h>
#include <filesystem>

#endif

#if defined(NL_PLATFORM_OSX)
constexpr const char* NL_LIB_EXT = ".dll";
#elif defined(NL_PLATFORM_LINUX)
constexpr const char* NL_LIB_EXT = ".so";
#endif

namespace Nightly
{
	void PluginManager::LoadActivePlugins()
	{
		NL_CORE_INFO("Loading plugins...", ENGINE);

		std::array<const char*, 1> activePluginNames = { "Test" };

		// Loop through all active plugin names and load them one by one
		for (const auto& pluginName : activePluginNames)
		{
			// Load plugin from file
			auto plugin = LoadPlugin(pluginName);

			// Continue to next one if an error occurred
			if (!plugin)
			{
				NL_CORE_FATAL("Failed to load plugin: " << pluginName, ENGINE);
				continue;
			}

			// Add to loaded plugins pool
			m_LoadedPlugins.push_back(plugin);
			NL_CORE_INFO("Loaded plugin: " << pluginName, ENGINE);

			plugin->OnLoad();
		}
	}

	Plugin* PluginManager::LoadPlugin(std::string_view name)
	{
		#if defined(NL_PLATFORM_WINDOWS)
		return WindowsPlatform::LoadPlugin(name);
		#else
		std::stringstream libName;
		libName << "./libNightlyPlugin_" << name << NL_LIB_EXT;

		// Load dynamic library file
		void* handle = dlopen(libName.str().c_str(), RTLD_NOW);
		if (!handle)
		{
			NL_CORE_FATAL("Plugin library file was not found: " << libName.str(), ENGINE);
			return nullptr;
		}

		// Get Plugin pointer to call its functions
		void* plugin = dlsym(handle, "GetPluginPtr");
		if (!plugin)
		{
			NL_CORE_FATAL("Failed to load symbols for plugin: " << libName.str(), ENGINE);
			return nullptr;
		}

		using PluginPtr = Plugin* (*)();
		auto fun = reinterpret_cast<PluginPtr>(plugin);

		return fun();
		#endif
	}

	void PluginManager::UnloadPlugins()
	{
		NL_CORE_INFO("Unloading plugins...", ENGINE);

		for (const auto& plugin : m_LoadedPlugins)
		{
			// TODO: Close library (dlclose) and create separate struct for plugins containing name, handle, ptr
			plugin->OnUnload();
		}
	}
}