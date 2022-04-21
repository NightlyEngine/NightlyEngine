#pragma once

#include "Core.h"

namespace Nightly
{
	// Specifies the source from which the message comes from.
	// Standalone games should always use LogSource::PLAYER.
	enum class LogSource
	{
		ENGINE,
		EDITOR,
		PLAYER,
	};

	// Specifies the severity of a log message.
	enum class LogSeverity
	{
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	// This class is responsible for logging messages to the console.
	class NL_API Log
	{
	public:
		// Logs an info message to the console.
		static void Info(const std::stringstream& message, const LogSource& source = LogSource::PLAYER);

		// Logs a warning message to the console.
		static void Warn(const std::stringstream& message, const LogSource& source = LogSource::PLAYER);

		// Logs an error message to the console.
		static void Error(const std::stringstream& message, const LogSource& source = LogSource::PLAYER);

		// Logs a fatal error message to the console.
		static void Fatal(const std::stringstream& message, const LogSource& source = LogSource::PLAYER);

	private:
		// Logs to the standard output using a custom template.
		static void LogIntern(const LogSeverity& severity, const LogSource& source, const std::stringstream& message);

		// Converts LogSeverity enum into string.
		static std::string LogSeverityToStr(const LogSeverity& severity);

		// Converts LogSource enum into string.
		static std::string LogSourceToStr(const LogSource& source);
	};

	// Base macro for logging.
	#define NL_LOG(message, source, severity)               \
            {                                               \
                std::stringstream _logStringStream;         \
                _logStringStream << message;                \
                Log::severity(_logStringStream, source);    \
            }

	// Macro for logging an info message to the console with the source being PLAYER.
	#define NL_INFO(message) NL_LOG(message, LogSource::PLAYER, Info)

	// Macro for logging a warning message to the console with the source being PLAYER.
	#define NL_WARN(message) NL_LOG(message, LogSource::PLAYER, Warn)

	// Macro for logging an error message to the console with the source being PLAYER.
	#define NL_ERROR(message) NL_LOG(message, LogSource::PLAYER, Error)

	// Macro for logging a fatal error message to the console with the source being PLAYER.
	#define NL_FATAL(message) NL_LOG(message, LogSource::PLAYER, Fatal)
}

