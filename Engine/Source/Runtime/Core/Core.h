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

// --------------- Smart Pointers --------------- //

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename ... Args>
auto MakeScope(Args&& ... args) -> decltype(auto)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename ... Args>
auto MakeRef(Args&& ... args) -> decltype(auto)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}