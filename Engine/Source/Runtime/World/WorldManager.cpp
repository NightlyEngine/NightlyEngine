#include "WorldManager.h"

#include "Core/Log.h"
#include "World.h"

namespace Nightly
{
	std::vector<std::shared_ptr<World>> WorldManager::m_WorldRegistry;
	std::shared_ptr<World> WorldManager::m_ActiveWorld;

	WorldManager::~WorldManager() = default;

	std::shared_ptr<World> WorldManager::CreateWorld(std::string_view name, uint64_t id)
	{
		const auto it = std::find_if(m_WorldRegistry.begin(), m_WorldRegistry.end(),
		                             [name](const std::shared_ptr<World>& element)
		                             {
			                             return element->GetName() == name;
		                             });

		// If a world with the same name exists, return nullptr
		if (it != m_WorldRegistry.end())
		{
			return nullptr;
		}

		// If we're serializing from a file, also specify the UUID
		auto world = id == 0 ? std::make_shared<World>(name) : std::make_shared<World>(name, id);
		m_WorldRegistry.push_back(world);

		NL_CORE_INFO("Created new world: " << name, ENGINE);

		return world;
	}

	bool WorldManager::LoadWorld(const std::shared_ptr<World>& newWorld)
	{
		if (m_ActiveWorld == newWorld) return false;

		const auto it = std::find(m_WorldRegistry.begin(), m_WorldRegistry.end(), newWorld);

		// If the world could not be found, return false
		if (it == m_WorldRegistry.end())
		{
			return false;
		}

		m_ActiveWorld = newWorld;

		NL_CORE_INFO("Loaded world: " << newWorld->GetName(), ENGINE);

		return true;
	}

	std::shared_ptr<World> WorldManager::FindWorld(std::string_view name)
	{
		auto world = std::find_if(m_WorldRegistry.begin(), m_WorldRegistry.end(),
		                          [name](const std::shared_ptr<World>& element)
		                          {
			                          return element->GetName() == name;
		                          });

		// Return nullptr if not found
		return world == m_WorldRegistry.end() ? nullptr : *world;
	}

	std::shared_ptr<World> WorldManager::FindWorldById(uint64_t id)
	{
		auto world = std::find_if(m_WorldRegistry.begin(), m_WorldRegistry.end(),
		                          [id](const std::shared_ptr<World>& element)
		                          {
			                          return element->GetUUID() == id;
		                          });

		// Return nullptr if not found
		return world == m_WorldRegistry.end() ? nullptr : *world;
	}

	bool WorldManager::RemoveWorld(const std::shared_ptr<World>& world)
	{
		const auto it = std::remove(m_WorldRegistry.begin(), m_WorldRegistry.end(), world);
		bool found = it != m_WorldRegistry.end();

		m_WorldRegistry.erase(it, m_WorldRegistry.end());
		return found;
	}
}