#include "EngineTime.h"

#include <chrono>
#include <ctime>
#include <iomanip>

namespace NL
{
	std::string EngineTime::GetTimeString()
	{
		auto now = std::chrono::system_clock::now();
		auto time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&time), "%X");
		return ss.str();
	}
}