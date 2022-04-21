#include "Log.h"

#include <chrono>
#include <ctime>

namespace Nightly
{
	void Log::Info(const std::stringstream& message, const LogSource& source)
	{
		LogIntern(LogSeverity::INFO, source, message);
	}

	void Log::Warn(const std::stringstream& message, const LogSource& source)
	{
		LogIntern(LogSeverity::WARNING, source, message);
	}

	void Log::Error(const std::stringstream& message, const LogSource& source)
	{
		LogIntern(LogSeverity::ERROR, source, message);
	}

	void Log::Fatal(const std::stringstream& message, const LogSource& source)
	{
		LogIntern(LogSeverity::FATAL, source, message);
	}

	void Log::LogIntern(const LogSeverity& severity, const LogSource& source, const std::stringstream& message)
	{
		// TODO: Move time related stuff into separate class
		time_t currentTime;
		struct tm* localTime;

		time(&currentTime);
		localTime = localtime(&currentTime);

		auto hour = std::to_string(localTime->tm_hour);
		auto min = std::to_string(localTime->tm_min);
		auto sec = std::to_string(localTime->tm_sec);

		if (hour.length() == 1) hour = "0" + hour;
		if (min.length() == 1) min = "0" + min;
		if (sec.length() == 1) sec = "0" + sec;

		std::stringstream timeStream;
		timeStream << hour << ":" << min << ":" << sec;

		std::cout << timeStream.str() << " " << LogSeverityToStr(severity) << " @" << LogSourceToStr(source) << ": " << message.str()
		          << std::endl;
	}

	std::string Log::LogSeverityToStr(const LogSeverity& severity)
	{
		switch (severity)
		{
			case LogSeverity::INFO:
				return "INFO";

			case LogSeverity::WARNING:
				return "WARNING";

			case LogSeverity::ERROR:
				return "ERROR";

			case LogSeverity::FATAL:
				return "FATAL";

			default:
				return "";
		}
	}

	std::string Log::LogSourceToStr(const LogSource& source)
	{
		switch (source)
		{
			case LogSource::ENGINE:
				return "Engine";

			case LogSource::EDITOR:
				return "Editor";

			case LogSource::PLAYER:
				return "Player";

			default:
				return "";
		}
	}
}