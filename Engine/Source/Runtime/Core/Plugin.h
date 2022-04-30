#pragma once

#include "EngineAPI.h"
#include "ITrackable.h"

#include "Platform/PlatformDetection.h"

#if defined(NL_PLATFORM_OSX)
#define NL_CDECL __cdecl
#elif defined(NL_PLATFORM_LINUX)
#define NL_CDECL __attribute__((__cdecl__))
#elif defined(NL_PLATFORM_WINDOWS)
#define NL_CDECL __declspec(dllexport)
#endif

// Defines the entry point for the plugin,
// so it can be called from the PluginManager.
#define NL_PLUGIN_IMPL(ClassName)                                           \
        extern "C" NL_CDECL Nightly::Plugin* GetPluginPtr()                 \
        {                                                                   \
            return static_cast<Nightly::Plugin*>(new Nightly::ClassName);   \
        }


namespace Nightly
{
	// This is a base class for plugins.
	class NL_API Plugin : public ITrackable<Plugin>
	{
	public:
		Plugin() = default;
		~Plugin() override = default;

		NL_DEFINE_TRACKABLE(Plugin);

		// Gets called when the plugin is loaded for the first time.
		virtual void OnLoad() = 0;

		// Gets called every frame.
		virtual void OnUpdate() = 0;

		// Gets called when the plugin is unloaded.
		virtual void OnUnload() = 0;
	};
}