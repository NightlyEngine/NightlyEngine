#pragma once

// Assert
#ifdef NDEBUG

#include "Log.h"

#define NL_ASSERT(condition, message, source) NL_CORE_FATAL(message, LogSource::source); return
#else

#include <cassert>

#define NL_ASSERT(condition, message, source) assert(condition && message)
#endif