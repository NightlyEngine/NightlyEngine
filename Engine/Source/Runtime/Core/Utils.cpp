#include "Utils.h"

namespace NL
{
	std::string Utils::ToUppercase(std::string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](uint8_t c)
		{
			return std::toupper(c);
		});

		return text;
	}

	std::string Utils::ToLowercase(std::string text)
	{
		std::transform(text.begin(), text.end(), text.begin(), [](uint8_t c)
		{
			return std::tolower(c);
		});

		return text;
	}
}