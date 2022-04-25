#include "PluginManager.h"

#include <dlfcn.h>
#include <array>

#include "Log.h"
#include "Plugin.h"

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
		std::stringstream libName;
		libName << "libNightlyPlugin_" << name << ".dylib";

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