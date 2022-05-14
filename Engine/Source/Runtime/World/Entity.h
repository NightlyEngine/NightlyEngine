#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"
#include "Core/UUID.h"
#include "Components/Component.h"
#include "Components/TransformComponent.h"

namespace Nightly
{
	class World;

	// An entity is an object that can be spawned in a world
	// that has components which define how it should behave.
	class NL_API Entity : public ITrackable<Entity>, public UUID
	{
	public:
		explicit Entity(std::string_view name = "New Entity", std::string_view tag = "Default")
				: m_Name(name), m_Tag(tag), UUID()
		{
		}

		Entity(std::string_view name, std::string_view tag, uint64_t uuid)
				: m_Name(name), m_Tag(tag), UUID(uuid)
		{
		}

		~Entity() override;

		NL_DEFINE_TRACKABLE(Entity);

		// Creates transform component and sets the attached world.
		void Initialize(const std::shared_ptr<World>& attachedWorld);

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

		// Returns the world that the entity is attached to.
		std::shared_ptr<World> GetWorld()
		{
			return m_AttachedWorld;
		}

		// Adds a component to the registry and returns
		// whether the operation was successful.
		template <typename T>
		bool AddComponent(std::shared_ptr<T> component)
		{
			// Prevent adding another transform component
			if (std::is_same_v<T, TransformComponent>)
			{
				return false;
			}

			component->m_ParentEntity = GetPointer();
			m_ComponentRegistry.push_back(component);

			return true;
		}

		// Removes the specified component from the registry and
		// returns whether the operation was successful.
		template <typename T>
		bool RemoveComponent()
		{
			// Prevent removing the transform component
			if (std::is_same_v<T, TransformComponent>)
			{
				return false;
			}

			auto component = GetComponent<T>();
			if (component)
			{
				auto it = std::remove(m_ComponentRegistry.begin(), m_ComponentRegistry.end(), component);
				bool found = it != m_ComponentRegistry.end();

				if (found)
				{
					m_ComponentRegistry.erase(it, m_ComponentRegistry.end());
					return true;
				}
			}

			return false;
		}

		// Returns the first component that matches
		// the specified type, otherwise nullptr.
		template <typename T>
		std::shared_ptr<T> GetComponent()
		{
			for (const auto& component : m_ComponentRegistry)
			{
				auto foundComponent = std::dynamic_pointer_cast<T>(component);
				if (foundComponent)
				{
					return foundComponent;
				}
			}

			return nullptr;
		}

		// Returns the transform component.
		std::shared_ptr<TransformComponent> Transform()
		{
			return GetComponent<TransformComponent>();
		}

	private:
		std::string m_Name;
		std::string m_Tag;
		std::shared_ptr<World> m_AttachedWorld;
		std::vector<std::shared_ptr<Component>> m_ComponentRegistry;

		[[nodiscard]] std::shared_ptr<Entity> GetPointer() const;
	};
}