#pragma once

#include "EngineAPI.h"

namespace Nightly
{
	// This class contains functions for retrieving time related information.
	class NL_API EngineTime
	{
	public:
		// Returns the current time in this format: HH:MM:SS.
		static std::string GetTimeString();
	};
}
