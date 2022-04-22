#pragma once

namespace Nightly
{
	// This class contains functions for retrieving time related information.
	class EngineTime
	{
	public:
		// Returns the current time in this format: HH:MM:SS.
		// If addLeadingZero is true, the string will be
		// formatted like this: 3:39:5 -> 03:39:05.
		static std::string GetTimeString(bool addLeadingZero);
	};
}
