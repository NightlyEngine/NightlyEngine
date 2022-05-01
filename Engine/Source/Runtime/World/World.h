#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"

namespace Nightly
{
	class Entity;

	using EntityList = std::vector<std::shared_ptr<Entity>>;

	class NL_API World : public ITrackable<World>
	{
	public:
		explicit World(std::string_view name = "New World")
				: m_Name(name)
		{
		}

		~World() override;

		NL_DEFINE_TRACKABLE(World);

		// Creates a new entity and adds it to the registry.
		std::shared_ptr<Entity> CreateEntity(std::string_view name = "New Entity", std::string_view tag = "Default", uint64_t uuid = 0);

		// Adds entity to the registry if it isn't already there,
		// returns false otherwise.
		bool AddEntity(const std::shared_ptr<Entity>& entity);

		// Scans the entire entity registry and returns
		// the first entity that matches the specified name.
		// Returns nullptr if it was not found.
		std::shared_ptr<Entity> FindEntity(std::string_view name);

		// Scans the entire entity registry and returns
		// the first entity that matches the specified tag.
		// Returns nullptr if it was not found.
		std::shared_ptr<Entity> FindEntityByTag(std::string_view tag);

		// Scans the entire entity registry and returns
		// the first entity that matches the specified id.
		// Returns nullptr if it was not found.
		std::shared_ptr<Entity> FindEntityById(uint64_t id);

		// Scans the entire entity registry and populates outList
		// with all entities that match the specified name.
		// If nothing was found, outList remains untouched.
		// Returns whether at least one entity was found.
		bool FindEntities(std::string_view name, EntityList& outList);

		// Scans the entire entity registry and populates outList
		// with all entities that match the specified tag.
		// If nothing was found, outList remains untouched.
		// Returns whether at least one entity was found.
		bool FindEntitiesByTag(std::string_view tag, EntityList& outList);

		// Removes entity from the registry
		// and returns whether the operation was successful.
		bool RemoveEntity(const std::shared_ptr<Entity>& entity);

		// Scans the entire entity registry and removes
		// all entities that match the specified name.
		// Returns whether the operation was successful.
		bool RemoveEntities(std::string_view name);

		// Scans the entire entity registry and removes
		// all entities that match the specified tag.
		// Returns whether the operation was successful.
		bool RemoveEntitiesByTag(std::string_view tag);

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
