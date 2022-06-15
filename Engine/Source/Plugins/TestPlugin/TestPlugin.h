#pragma once

#include "Runtime/Core/Plugin.h"

namespace Nightly
{
	class NL_PLUGIN_API TestPlugin : public Plugin
	{
		~TestPlugin() override = default;

		void OnLoad() override;
		void OnUpdate() override;
		void OnUnload() override;
	};
}

NL_PLUGIN_IMPL(TestPlugin)
