#include "MemoryManager.h"

#include "Log.h"

#define NL_MEGABYTES 1000000
#define NL_KILOBYTES 1000

namespace NL
{
	void MemoryManager::LogUsage()
	{
		auto current = (float) m_CurrentAllocation;
		const char* type = "BYTES";

		// Convert to bigger types to avoid long numbers
		if (m_CurrentAllocation >= NL_MEGABYTES)
		{
			current = (float) m_CurrentAllocation / NL_MEGABYTES;
			type = "MB";
		}
		else if (m_CurrentAllocation >= NL_KILOBYTES)
		{
			current = (float) m_CurrentAllocation / NL_KILOBYTES;
			type = "KB";
		}

		NL_CORE_INFO("Current memory usage: " << current << " " << type, ENGINE);
	}

	void MemoryManager::LogAllocation(std::string_view className, size_t size)
	{
		NL_CORE_INFO(className << " Class: Allocated " << size << " bytes", ENGINE);
	}

	void MemoryManager::LogDeallocation(std::string_view className, size_t size)
	{
		NL_CORE_INFO(className << ": Freed " << size << " bytes", ENGINE);
	}
}