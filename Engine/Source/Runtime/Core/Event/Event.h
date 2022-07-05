#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"

// Implements the GetName() function.
#define NL_DEFINE_EVENT(ClassName) NL_NODISCARD virtual std::string GetName() const override { return #ClassName; } \
static const inline std::string EventType = #ClassName

namespace NL
{
	struct NL_API Event
	{
		Event() = default;
		virtual ~Event() = default;

		NL_NODISCARD virtual std::string GetName() const = 0;

		static const inline std::string EventType = "Unknown Event";
	};

	// Parameter for event callback functions
	typedef const Event& EventFun;
}
