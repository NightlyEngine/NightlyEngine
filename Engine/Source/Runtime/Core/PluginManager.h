#pragma once

#include "EngineAPI.h"

namespace Nightly
{
	class Plugin;

	// Contains information about a plugin like
	// the pointer to the class and its handle.
	struct PluginSpecification
	{
		PluginSpecification() = default;

		Plugin* pluginPtr = nullptr;
		void* handle = nullptr;
	};

	// This class manages and loads native
	// plugins as dynamic libraries.
	class NL_API PluginManager
	{
	public:
		PluginManager() = default;
		~PluginManager() = default;

		// Loads all plugins that are marked as active.
		// These are hard-coded for now.
		static void LoadActivePlugins();

		// Calls OnUpdate() on all active plugins.
		static void UpdatePlugins();

		// Unloads all plugins so the memory can be released.
		static void UnloadPlugins();

	private:
		static PluginSpecification LoadPlugin(std::string_view name);

		static inline std::vector<PluginSpecification> m_LoadedPlugins;
	};
}
