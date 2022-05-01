#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"

namespace Nightly
{
	// An entity is an object that can be spawned in a world
	// that has components which define how it should behave.
	class NL_API Entity : public ITrackable<Entity>
	{
	public:
		explicit Entity(std::string_view name = "New Entity", std::string_view tag = "Default")
				: m_Name(name), m_Tag(tag)
		{
		}

		~Entity() override = default;

		NL_DEFINE_TRACKABLE(Entity);

		[[nodiscard]] const std::string& GetName() const
		{
			return m_Name;
		}

		void SetName(const std::string& newName)
		{
			m_Name = newName;
		}

		[[nodiscard]] const std::string& GetTag() const
		{
			return m_Tag;
		}

		void SetTag(const std::string& newTag)
		{
			m_Tag = newTag;
		}

	private:
		std::string m_Name;
		std::string m_Tag;
	};
}