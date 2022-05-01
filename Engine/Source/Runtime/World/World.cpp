#include "World.h"

#include "Core/Log.h"
#include "Entity.h"

namespace Nightly
{
	World::~World() = default;

	void World::AddEntity(const std::shared_ptr<Entity>& entity)
	{
		m_EntityRegistry.push_back(entity);
		NL_CORE_INFO("Registered entity: " << entity->GetName(), ENGINE);
	}

	std::shared_ptr<Entity> World::FindEntity(std::string_view name)
	{
		for (const auto& entity : m_EntityRegistry)
		{
			if (entity->GetName() == name)
			{
				return entity;
			}
		}

		return nullptr;
	}

	std::shared_ptr<Entity> World::FindEntityByTag(std::string_view tag)
	{
		for (const auto& entity : m_EntityRegistry)
		{
			if (entity->GetTag() == tag)
			{
				return entity;
			}
		}

		return nullptr;
	}

	bool World::FindEntities(std::string_view name, EntityList& outList)
	{
		bool found = false;

		for (const auto& entity : m_EntityRegistry)
		{
			if (entity->GetName() == name)
			{
				found = true;
				outList.push_back(entity);
			}
		}

		return found;
	}

	bool World::FindEntitiesByTag(std::string_view tag, EntityList& outList)
	{
		bool found = false;

		for (const auto& entity : m_EntityRegistry)
		{
			if (entity->GetTag() == tag)
			{
				found = true;
				outList.push_back(entity);
			}
		}

		return found;
	}

	void World::RemoveEntity(std::shared_ptr<Entity>& entity)
	{
		auto it = std::remove(m_EntityRegistry.begin(), m_EntityRegistry.end(), entity);

		if (it == m_EntityRegistry.end())
		{
			NL_CORE_ERROR("Failed to remove entity because it was not found in the registry: " << entity->GetName(), ENGINE);
			return;
		}

		m_EntityRegistry.erase(it);
		NL_CORE_INFO("Removed entity: " << entity->GetName(), ENGINE);
	}
}