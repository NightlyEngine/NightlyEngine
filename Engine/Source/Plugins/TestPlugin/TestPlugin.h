#pragma once

#include "Runtime/Core/Plugin.h"
#include "Runtime/Platform/PlatformDetection.h"

namespace Nightly
{
	class NL_API TestPlugin : public Plugin
	{
		void OnLoad() override;
		void OnUpdate() override;
		void OnUnload() override;
	};
}

#if defined(NL_PLATFORM_OSX)
#define NL_CDECL __cdecl
#elif defined(NL_PLATFORM_LINUX)
#define NL_CDECL __attribute__((__cdecl__))
#elif defined(NL_PLATFORM_WINDOWS)
#define NL_CDECL __declspec(dllexport) __stdcall
#endif

// TODO: Move into macro
extern "C" NL_CDECL Nightly::Plugin* GetPluginPtr()
{
	// TODO: Delete variable
	return static_cast<Nightly::Plugin*>(new Nightly::TestPlugin);
}
