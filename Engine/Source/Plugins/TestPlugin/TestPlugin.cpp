#include "TestPlugin.h"

#include "Runtime/Core/Log.h"

namespace Nightly
{
	void TestPlugin::OnLoad()
	{
		NL_CORE_INFO("Test plugin loaded successfully!", PLUGIN);
	}

	void TestPlugin::OnUpdate()
	{
		NL_CORE_INFO("Updating plugin...", PLUGIN);
	}

	void TestPlugin::OnUnload()
	{
		NL_CORE_INFO("Unloading plugin...", PLUGIN);
	}
}