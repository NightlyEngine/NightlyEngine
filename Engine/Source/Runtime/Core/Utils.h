#pragma once

#include "EngineAPI.h"

// Concatenates multiple data types into one string.
#define NL_TEXT(args, ...) Utils::ToText(args, ##__VA_ARGS__)

namespace NL
{
	class NL_API Utils
	{
	public:
		static std::string ToUppercase(std::string text);
		static std::string ToLowercase(std::string text);

		template <typename T>
		static std::string ToText(const T& text)
		{
			std::stringstream stream;
			stream << text;
			return stream.str();
		}

		template <typename T, typename ... Args>
		static std::string ToText(const T& text, const Args& ... args)
		{
			return ToText(text) + ToText(args...);
		}
	};
}
