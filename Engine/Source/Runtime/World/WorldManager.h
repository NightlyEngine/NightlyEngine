#pragma once

#include "Core/EngineAPI.h"
#include "Core/Core.h"

namespace NL
{
	class World;

	// Keeps track of all worlds and provides functions for working with them.
	class NL_API WorldManager
	{
	public:
		WorldManager() = default;
		~WorldManager();

		// Creates a new world if another one with
		// the specified name doesn't exist already,
		// returns nullptr otherwise.
		static Ref<World> CreateWorld(std::string_view name, uint64_t id = 0);

		// Sets the active world to newWorld if it was
		// found in the registry, returns false otherwise.
		// If newWorld is already the active world, false is returned.
		static bool LoadWorld(const Ref<World>& newWorld);

		// Scans the entire world registry and returns
		// the world that matches the specified name.
		// Returns nullptr if it was not found.
		static Ref<World> FindWorld(std::string_view name);

		// Scans the entire world registry and returns
		// the world that matches the specified id.
		// Returns nullptr if it was not found.
		static Ref<World> FindWorldById(uint64_t id);

		// Removes the world from the registry if
		// it was found, returns false otherwise.
		static bool RemoveWorld(const Ref<World>& world);

		// Returns the currently loaded world.
		static Ref<World>& GetActiveWorld()
		{
			return m_ActiveWorld;
		}

	private:
		static std::vector<Ref<World>> m_WorldRegistry;
		static Ref<World> m_ActiveWorld;
	};
}
