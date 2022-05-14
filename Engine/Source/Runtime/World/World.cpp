#include "World.h"

#include "Core/Log.h"
#include "Entity.h"
#include "WorldManager.h"

namespace Nightly
{
	World::~World() = default;

	std::shared_ptr<Entity> World::CreateEntity(std::string_view name, std::string_view tag, uint64_t uuid)
	{
		std::shared_ptr<Entity> entity;

		if (uuid == 0)
		{
			entity = std::make_shared<Entity>(name, tag);
		}
		else
		{
			entity = std::make_shared<Entity>(name, tag, uuid);
		}

		m_EntityRegistry.push_back(entity);
		entity->Initialize(WorldManager::FindWorldById(GetUUID()));

		return entity;
	}

	bool World::AddEntity(const std::shared_ptr<Entity>& entity)
	{
		auto it = std::find(m_EntityRegistry.begin(), m_EntityRegistry.end(), entity);

		if (it != m_EntityRegistry.end())
		{
			return false;
		}

		m_EntityRegistry.push_back(entity);
		NL_CORE_INFO("Registered entity: " << entity->GetName(), ENGINE);

		return true;
	}

	std::shared_ptr<Entity> World::FindEntity(std::string_view name)
	{
		auto entity = std::find_if(m_EntityRegistry.begin(), m_EntityRegistry.end(),
		                           [name](const std::shared_ptr<Entity>& element)
		                           {
			                           return element->GetName() == name;
		                           });

		// Return nullptr if not found
		return entity == m_EntityRegistry.end() ? nullptr : *entity;
	}

	std::shared_ptr<Entity> World::FindEntityByTag(std::string_view tag)
	{
		auto entity = std::find_if(m_EntityRegistry.begin(), m_EntityRegistry.end(),
		                           [tag](const std::shared_ptr<Entity>& element)
		                           {
			                           return element->GetTag() == tag;
		                           });

		// Return nullptr if not found
		return entity == m_EntityRegistry.end() ? nullptr : *entity;
	}

	std::shared_ptr<Entity> World::FindEntityById(uint64_t id)
	{
		auto entity = std::find_if(m_EntityRegistry.begin(), m_EntityRegistry.end(),
		                           [id](const std::shared_ptr<Entity>& element)
		                           {
			                           return element->GetUUID() == id;
		                           });

		// Return nullptr if not found
		return entity == m_EntityRegistry.end() ? nullptr : *entity;
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

	bool World::RemoveEntity(const std::shared_ptr<Entity>& entity)
	{
		const auto it = std::remove(m_EntityRegistry.begin(), m_EntityRegistry.end(), entity);
		bool found = it != m_EntityRegistry.end();

		m_EntityRegistry.erase(it, m_EntityRegistry.end());
		return found;
	}

	bool World::RemoveEntities(std::string_view name)
	{
		const auto it = std::remove_if(m_EntityRegistry.begin(), m_EntityRegistry.end(),
		                               [name](const std::shared_ptr<Entity>& element)
		                               {
			                               return element->GetName() == name;
		                               });

		bool found = it != m_EntityRegistry.end();

		m_EntityRegistry.erase(it, m_EntityRegistry.end());
		return found;
	}

	bool World::RemoveEntitiesByTag(std::string_view tag)
	{
		const auto it = std::remove_if(m_EntityRegistry.begin(), m_EntityRegistry.end(),
		                               [tag](const std::shared_ptr<Entity>& element)
		                               {
			                               return element->GetTag() == tag;
		                               });

		bool found = it != m_EntityRegistry.end();

		m_EntityRegistry.erase(it, m_EntityRegistry.end());
		return found;
	}
}