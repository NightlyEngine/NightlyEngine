#include "Log.h"

#include "EngineTime.h"

#ifdef NL_PLATFORM_WINDOWS

#include "Platform/WindowsPlatform.h"

#endif

namespace NL
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
		std::string currentTime = EngineTime::GetTimeString();
		std::string color, clearColor;

#ifdef NL_PLATFORM_WINDOWS
		WindowsPlatform::SetConsoleColor(severity);
#else
		color = GetSeverityColor(severity);
		clearColor = ConsoleColors::Clear;
#endif

		std::cout << color << currentTime << " " << LogSeverityToStr(severity) << " @" << LogSourceToStr(source) << ": "
		          << message.str() << clearColor << "\n";

		ConsoleMessageMetadata metadata(message.str(), currentTime, severity, source);
		m_LogBuffer.push_back(metadata);

		// Clear color after logging on Windows
		#ifdef NL_PLATFORM_WINDOWS
		WindowsPlatform::ClearConsoleColor();
		#endif
	}

	std::string Log::GetSeverityColor(const LogSeverity& severity)
	{
		switch (severity)
		{
			case LogSeverity::INFO:
				return ConsoleColors::Clear;

			case LogSeverity::WARNING:
				return ConsoleColors::Yellow;

			case LogSeverity::ERROR:
				return ConsoleColors::Red;

			case LogSeverity::FATAL:
				return ConsoleColors::FatalRed;

			default:
				return "";
		}
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

			case LogSource::PLUGIN:
				return "Plugin";

			default:
				return "";
		}
	}
}