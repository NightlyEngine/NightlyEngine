#pragma once

// Assert
#ifdef NDEBUG

#include "Log.h"

// Creates a string stream for concatenating multiple data types.
#define NL_TEXT(string) std::stringstream("").str() << string

#define NL_ASSERT(condition, message, source)           \
do                                                      \
{                                                       \
	if(!(condition)) NL_CORE_FATAL(message, source);    \
}                                                       \
while(0)
#else

#include <cassert>

// Creates a string stream for concatenating multiple data types.
#define NL_TEXT(string) std::stringstream("") << string

#define NL_ASSERT(condition, message, source) assert(condition && message)
#endif

#define NL_NODISCARD [[nodiscard]]

template <typename T>
using Ref = std::shared_ptr<T>;