#include "World.h"

#include "Core/Log.h"
#include "Entity.h"

namespace Nightly
{
	World::~World() = default;

	void World::AddEntity(std::shared_ptr<Entity>& entity)
	{
		m_EntityRegistry.push_back(entity);
		NL_CORE_INFO("Registered entity: " << entity->GetName(), ENGINE);
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