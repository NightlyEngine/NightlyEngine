#pragma once

#include "EngineAPI.h"

namespace Nightly
{
	class Plugin;

	// NOT COMPLETE, MISSING WINDOWS SUPPORT.
	// This class manages and loads native
	// plugins as dynamic libraries.
	class NL_API PluginManager
	{
	public:
		PluginManager() = default;
		~PluginManager() = default;

		static void LoadActivePlugins();

		static void UnloadPlugins();

	private:
		static Plugin* LoadPlugin(std::string_view name);

		static inline std::vector<Plugin*> m_LoadedPlugins;
	};
}
