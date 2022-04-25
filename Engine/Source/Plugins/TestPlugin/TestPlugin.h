#pragma once

#include "Runtime/Core/Plugin.h"

namespace Nightly
{
	class NL_API TestPlugin : public Plugin
	{
		void OnLoad() override;
		void OnUpdate() override;
		void OnUnload() override;
	};
}

// TODO: Move into macro
extern "C" __cdecl Nightly::Plugin* GetPluginPtr()
{
	// TODO: Delete variable
	return static_cast<Nightly::Plugin*>(new Nightly::TestPlugin);
}
