#pragma once

#include "EngineAPI.h"

namespace Nightly
{
	// This is a base class for plugins.
	class NL_API Plugin
	{
	public:
		Plugin() = default;
		virtual ~Plugin() = default;

		virtual void OnLoad() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnUnload() = 0;
	};
}