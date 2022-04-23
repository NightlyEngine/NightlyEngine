#pragma once

#include "Platform/PlatformDetection.h"

// Export or import library
#if defined(NL_EXPORT_DLL)

#if defined(NL_PLATFORM_WINDOWS)
#define NL_API __declspec(dllexport)
#elif defined(NL_PLATFORM_OSX) || defined(NL_PLATFORM_LINUX)
#define NL_API __attribute__((visibility("default")))
#else
#define NL_API
#endif

#else
#define NL_API
#endif

// Assert
#ifdef NDEBUG

#include "Log.h"

#define NL_ASSERT(condition, message, source) NL_CORE_FATAL(message, LogSource::source)
#else
#define NL_ASSERT(condition, message, source) assert(condition && message)
#endif