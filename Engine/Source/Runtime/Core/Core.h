#pragma once

#define NL_NODISCARD [[nodiscard]]

template <typename T>
using Ref = std::shared_ptr<T>;

// Creates a string stream for concatenating multiple data types.
#define NL_TEXT(string) std::stringstream("") << string

// Assert
#ifdef NDEBUG

#include "Log.h"

#define NL_ASSERT(condition, message, source)           \
do                                                      \
{                                                       \
	if(!(condition)) NL_CORE_FATAL(message, source);    \
}                                                       \
while(0)
#else

#include <cassert>

#define NL_ASSERT(condition, message, source) assert(condition && message)
#endif