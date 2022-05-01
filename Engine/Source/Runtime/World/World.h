#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"

namespace Nightly
{
	class Entity;

	class NL_API World : public ITrackable<World>
	{
	public:
		explicit World(std::string_view name = "New World")
				: m_Name(name)
		{
		}

		~World() override;

		NL_DEFINE_TRACKABLE(World);

		// Adds entity to the world registry.
		void AddEntity(std::shared_ptr<Entity>& entity);

		// Removes entity from the world registry.
		void RemoveEntity(std::shared_ptr<Entity>& entity);

		[[nodiscard]] const std::string& GetName() const
		{
			return m_Name;
		}

		void SetName(const std::string& newName)
		{
			m_Name = newName;
		}

	private:
		std::string m_Name;
		std::vector<std::shared_ptr<Entity>> m_EntityRegistry;
	};
}
