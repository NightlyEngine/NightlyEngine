#pragma once

#include "EngineAPI.h"
#include "Core.h"

namespace NL
{
	// Specifies the source from which the message comes from.
	// Standalone games should always use LogSource::PLAYER.
	enum LogSource : uint8_t
	{
		ENGINE,
		EDITOR,
		PLAYER,
		PLUGIN
	};

	// Specifies the severity of a log message.
	enum LogSeverity : uint8_t
	{
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	struct ConsoleMessageMetadata
	{
		ConsoleMessageMetadata(std::string content, std::string timestamp, const LogSeverity& severity, const LogSource& source)
				: m_Content(std::move(content)), m_Timestamp(std::move(timestamp)), m_Severity(severity), m_Source(source)
		{
		}

		NL_NODISCARD const std::string& GetContent() const
		{
			return m_Content;
		}

		NL_NODISCARD const std::string& GetTimestamp() const
		{
			return m_Timestamp;
		}

		NL_NODISCARD const LogSeverity& GetSeverity() const
		{
			return m_Severity;
		}

		NL_NODISCARD const LogSource& GetSource() const
		{
			return m_Source;
		}

	private:
		const std::string m_Content;
		const std::string m_Timestamp;
		const LogSeverity m_Severity;
		const LogSource m_Source;
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

		// Returns all messages that have been printed to the standard output by this class.
		static const std::vector<ConsoleMessageMetadata>& GetLogBuffer()
		{
			return m_LogBuffer;
		}

		static void ClearLogBuffer()
		{
			m_LogBuffer.clear();
		}

		// Converts LogSeverity enum into string.
		static std::string LogSeverityToStr(const LogSeverity& severity);

		// Converts LogSource enum into string.
		static std::string LogSourceToStr(const LogSource& source);

	private:
		// Contains colors that can be used when printing messages to the console.
		struct ConsoleColors
		{
			static constexpr const char* Clear = "\033[0m";
			static constexpr const char* Yellow = "\033[33m";
			static constexpr const char* Red = "\033[31m";
			static constexpr const char* FatalRed = "\033[1m\033[31m";
		};

		// Logs to the standard output using a custom template.
		static void LogIntern(const LogSeverity& severity, const LogSource& source, const std::stringstream& message);

		// Returns a console color based on the severity.
		static std::string GetSeverityColor(const LogSeverity& severity);

		static inline std::vector<ConsoleMessageMetadata> m_LogBuffer;
	};

	// Base macro for logging messages to the console.
	#define NL_LOG(message, source, severity)                   \
            do {                                                \
                std::stringstream _logStringStream;             \
                _logStringStream << message;                    \
                Log::severity(_logStringStream, source);        \
            } while(0)

	// Macro for logging an info message to the console with the source being PLAYER.
	#define NL_INFO(message) NL_LOG(message, NL::LogSource::PLAYER, Info)

	// Same as NL_INFO, but with the option to specify a source.
	#define NL_CORE_INFO(message, source) NL_LOG(message, NL::LogSource::source, Info)

	// Macro for logging a warning message to the console with the source being PLAYER.
	#define NL_WARN(message) NL_LOG(message, NL::LogSource::PLAYER, Warn)

	// Same as NL_WARN, but with the option to specify a source.
	#define NL_CORE_WARN(message, source) NL_LOG(message, NL::LogSource::source, Warn)

	// Macro for logging an error message to the console with the source being PLAYER.
	#define NL_ERROR(message) NL_LOG(message, NL::LogSource::PLAYER, Error)

	// Same as NL_ERROR, but with the option to specify a source.
	#define NL_CORE_ERROR(message, source) NL_LOG(message, NL::LogSource::source, Error)

	// Macro for logging a fatal error message to the console with the source being PLAYER.
	#define NL_FATAL(message) NL_LOG(message, NL::LogSource::PLAYER, Fatal)

	// Same as NL_FATAL, but with the option to specify a source.
	#define NL_CORE_FATAL(message, source) NL_LOG(message, NL::LogSource::source, Fatal)

	// Converts Vec2 into text.
	#define NL_VEC2_TEXT(vector) "( " << (vector).x << " | " << (vector).y << " )"

	// Converts Vec3 into text.
	#define NL_VEC3_TEXT(vector) "( " << (vector).x << " | " << (vector).y << " | " << (vector).z << " )"

	// Converts Vec4 into text.
	#define NL_VEC4_TEXT(vector) "( " << (vector).x << " | " << (vector).y << " | " << (vector).z << " | " << (vector).w << " )"
}

