#pragma once

#ifdef NL_EXPORT    // NL_EXPORT

#ifdef _WIN32       // _WIN32
#define NL_API __declspec(dllexport)
#elif __APPLE__ || __linux__
#define NL_API __attribute__((visibility("default")))
#endif              // _WIN32

#else               // NL_EXPORT NOT DEFINED

#ifdef _WIN32       // _WIN32
#define NL_API __declspec(dllimport)
#elif __APPLE__ || __linux__
#define NL_API
#endif              // _WIN32

#endif              // NL_EXPORT