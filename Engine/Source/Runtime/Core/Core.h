#pragma once

// Assert
#ifdef NDEBUG

#include "Log.h"

#define NL_ASSERT(condition, message, source) NL_CORE_FATAL(message, LogSource::source); return
#else

#include <cassert>

#define NL_ASSERT(condition, message, source) assert(condition && message)
#endif

// Creates a string stream for concatenating multiple data types.
#define NL_TEXT(string) std::stringstream("") << string