#pragma once

#include "MemoryManager.h"

// Sets the TrackableClassName variable that is used by ITrackable.
#define NL_DEFINE_TRACKABLE(className) static constexpr std::string_view TrackableClassName = #className

namespace Nightly
{
	// Tracks the heap memory usage of its derived class
	// and reports it to MemoryManager for profiling.
	// The template argument must be the derived class.
	// You have to define the class name using the
	// NL_DEFINE_TRACKABLE macro somewhere inside the
	// class body under a public field.
	template <typename T>
	struct ITrackable
	{
		virtual ~ITrackable() = default;

		void* operator new(size_t size)
		{
			MemoryManager::Allocate(T::TrackableClassName, size);
			return malloc(size);
		}

		void operator delete(void* memory, size_t size) noexcept
		{
			MemoryManager::Free(T::TrackableClassName, size);
			free(memory);
		}
	};
}
