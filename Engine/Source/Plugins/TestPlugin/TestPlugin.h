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

NL_PLUGIN_IMPL(TestPlugin)
