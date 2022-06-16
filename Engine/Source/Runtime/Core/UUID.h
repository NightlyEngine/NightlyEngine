#pragma once

#include "EngineAPI.h"
#include "Core.h"

namespace NL
{
	class NL_API UUID
	{
	public:
		UUID();

		explicit UUID(uint64_t uuid) : m_UUID(uuid)
		{
		}

		virtual ~UUID() = default;

		explicit operator uint64_t() const
		{
			return m_UUID;
		}

		NL_NODISCARD const uint64_t& GetUUID() const
		{
			return m_UUID;
		}

	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template <>
	struct hash<NL::UUID>
	{
		std::size_t operator()(const NL::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t) uuid);
		}
	};
}
