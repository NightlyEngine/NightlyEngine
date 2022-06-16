#include "UUID.h"

#include <random>

namespace NL
{
	static std::random_device m_Device;
	static std::mt19937_64 m_Engine(m_Device());
	static std::uniform_int_distribution<uint64_t> m_Distribution;

	UUID::UUID() : m_UUID(m_Distribution(m_Engine))
	{
	}
}