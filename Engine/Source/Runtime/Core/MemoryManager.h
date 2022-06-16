#pragma once

#include "EngineAPI.h"

namespace NL
{
	// Keeps track of the total heap allocated memory.
	class NL_API MemoryManager
	{
	public:
		MemoryManager() = default;
		~MemoryManager() = default;

		// Increases the current allocation by the size
		// and logs the current state to the console.
		static void Allocate(std::string_view className, size_t size)
		{
			LogAllocation(className, size);
			m_CurrentAllocation += size;
			LogUsage();
		}

		// Decreases the current allocation by the size
		// and logs the current state to the console.
		static void Free(std::string_view className, size_t size)
		{
			LogDeallocation(className, size);
			m_CurrentAllocation -= size;
			LogUsage();
		}

	private:
		// Prints the current heap memory usage to the console.
		static void LogUsage();

		// Prints how much memory the class allocated in the heap.
		static void LogAllocation(std::string_view className, size_t size);

		// Prints how much memory was freed by the class in the heap.
		static void LogDeallocation(std::string_view className, size_t size);

		static inline size_t m_CurrentAllocation;
	};
}
