#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"
#include "Core/UUID.h"
#include "Core/Core.h"

// Implements the GetName() function.
#define NL_DEFINE_COMPONENT(ClassName) NL_NODISCARD virtual std::string GetName() const override { return #ClassName; }

namespace Nightly
{
	class Entity;

	// Base component class.
	struct NL_API Component : public ITrackable<Component>, public UUID
	{
		Component() = default;
		~Component() override;

		NL_DEFINE_TRACKABLE(Component);

		NL_NODISCARD virtual std::string GetName() const = 0;

		// TODO: Call this function
		virtual void OnDestroy()
		{
		}

		std::shared_ptr<Entity> GetEntity()
		{
			return m_AttachedEntity;
		}

	private:
		std::shared_ptr<Entity> m_AttachedEntity;

		friend class Entity;
	};
}
