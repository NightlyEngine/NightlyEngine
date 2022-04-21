#pragma once

#ifdef NL_EXPORT_DLL    // NL_EXPORT_DLL

#ifdef _WIN32       	// _WIN32
#define NL_API __declspec(dllexport)
#elif __APPLE__ || __linux__
#define NL_API __attribute__((visibility("default")))
#endif              	// _WIN32

#else               	// NL_EXPORT_DLL NOT DEFINED

#ifdef _WIN32           // _WIN32
#define NL_API 
#elif __APPLE__ || __linux__
#define NL_API
#endif                  // _WIN32

#endif                  // NL_EXPORT_DLL
