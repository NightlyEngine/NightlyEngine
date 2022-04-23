#include "EngineTime.h"

#include <chrono>
#include <ctime>
#include <iomanip>

namespace Nightly
{
	std::string EngineTime::GetTimeString()
	{
		auto now = std::chrono::system_clock::now();
		long in_time_t = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&in_time_t), "%X");
		return ss.str();
	}
}