#pragma once

namespace Nightly
{
	// This class contains functions for retrieving time related information.
	class EngineTime
	{
	public:
		// Returns the current time in this format: HH:MM:SS.
		static std::string GetTimeString();
	};
}
