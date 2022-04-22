#pragma once

#if defined(_WIN32) || defined(WIN32) || defined(__WIN32__) || defined(__NT__)

#ifdef _WIN64
#define NL_PLATFORM_WINDOWS
#else
#error "32-bit version of Windows is not supported!"
#endif

#elif defined(__ANDROID__)
#define NL_PLATFORM_ANDROID
#elif defined(__linux__)
#define NL_PLATFORM_LINUX
#elif defined(__APPLE__)

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE && TARGET_OS_SIMULATOR
#define NL_PLATFORM_IPHONE_SIMULATOR
#elif TARGET_OS_IPHONE
#define NL_PLATFORM_IPHONE
#elif TARGET_OS_OSX
#define NL_PLATFORM_OSX
#endif

#elif defined(__unix)
#define NL_PLATFORM_UNIX
#elif defined(__posix)
#define NL_PLATFORM_POSIX
#else
#error "Platform is not supported!"
#endif
