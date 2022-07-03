#pragma once

#define NL_NODISCARD [[nodiscard]]

#define NL_BIT(x) (1 << x)

// --------------- Assert --------------- //
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