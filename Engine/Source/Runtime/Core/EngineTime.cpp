#include "EngineTime.h"

#include <chrono>
#include <ctime>

namespace Nightly
{
	std::string EngineTime::GetTimeString(bool addLeadingZero)
	{
		time_t currentTime;
		struct tm* localTime;

		time(&currentTime);
		localTime = localtime(&currentTime);

		auto hour = std::to_string(localTime->tm_hour);
		auto min = std::to_string(localTime->tm_min);
		auto sec = std::to_string(localTime->tm_sec);

		// Add leading zeros
		if (addLeadingZero)
		{
			if (hour.length() == 1) hour = "0" + hour;
			if (min.length() == 1) min = "0" + min;
			if (sec.length() == 1) sec = "0" + sec;
		}

		return hour + ":" + min + ":" + sec;
	}
}