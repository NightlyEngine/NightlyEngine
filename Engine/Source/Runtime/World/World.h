#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"
#include "Entity.h"

namespace Nightly
{
	class NL_API World : public ITrackable<World>
	{
	public:
		World() = default;
		~World() override = default;

		NL_DEFINE_TRACKABLE(World);

		// Adds entity to the world registry.
		void AddEntity(std::shared_ptr<Entity>& entity);

		// Removes entity from the world registry.
		void RemoveEntity(std::shared_ptr<Entity>& entity);

	private:
		std::vector<std::shared_ptr<Entity>> m_EntityRegistry;
	};
}
