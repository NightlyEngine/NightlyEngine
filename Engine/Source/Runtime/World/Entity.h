#pragma once

#include "Core/EngineAPI.h"
#include "Core/ITrackable.h"
#include "Core/UUID.h"
#include "Core/Core.h"

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
				: UUID(), m_Name(name), m_Tag(tag)
		{
		}

		Entity(std::string_view name, std::string_view tag, uint64_t uuid)
				: UUID(uuid), m_Name(name), m_Tag(tag)
		{
		}

		~Entity() override;

		NL_DEFINE_TRACKABLE(Entity);

		// Creates transform component and sets the attached world.
		void Initialize(const Ref<World>& attachedWorld);

		NL_NODISCARD const std::string& GetName() const
		{
			return m_Name;
		}

		void SetName(const std::string& newName)
		{
			m_Name = newName;
		}

		NL_NODISCARD const std::string& GetTag() const
		{
			return m_Tag;
		}

		void SetTag(const std::string& newTag)
		{
			m_Tag = newTag;
		}

		// Returns the world that the entity is attached to.
		Ref<World> GetWorld()
		{
			return m_AttachedWorld;
		}

		// Attaches this entity to the parent entity.
		void SetParent(const Ref<Entity>& parent)
		{
			if (!parent) return;

			m_ParentEntity = parent;
			parent->m_ChildEntities.push_back(GetPointer());

			Transform()->SetPosition(Transform()->GetPosition() - parent->Transform()->GetPosition());
		}

		// Returns the parent that this entity is attached to.
		Ref<Entity> GetParent()
		{
			return m_ParentEntity;
		}

		// Detaches the entity from its parent.
		void Detach()
		{
			if (!m_ParentEntity) return;

			Transform()->SetPosition(Transform()->GetPosition() + GetParent()->Transform()->GetPosition());

			auto& vec = m_ParentEntity->m_ChildEntities;
			auto it = std::remove(vec.begin(), vec.end(), GetPointer());
			m_ParentEntity->m_ChildEntities.erase(it, vec.end());
			m_ParentEntity = nullptr;
		}

		// Returns the child entity by a specified index, which is 0 by default.
		// Returns nullptr if the index was out of bounds.
		Ref<Entity> GetChild(uint64_t index = 0)
		{
			return m_ChildEntities.size() > index ? m_ChildEntities[index] : nullptr;
		}

		// Returns all child entities.
		std::vector<Ref<Entity>>& GetChildren()
		{
			return m_ChildEntities;
		}

		// Returns whether the entity has any children.
		bool HasChildren()
		{
			return !m_ChildEntities.empty();
		}

		// Adds a component to the registry and returns
		// whether the operation was successful.
		template <typename T>
		bool AddComponent(Ref<T> component)
		{
			// Prevent adding another transform component
			if (std::is_same_v<T, TransformComponent>)
			{
				return false;
			}

			component->m_AttachedEntity = GetPointer();
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
		Ref<T> GetComponent()
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
		Ref<TransformComponent> Transform()
		{
			return GetComponent<TransformComponent>();
		}

	private:
		std::string m_Name;
		std::string m_Tag;

		Ref<World> m_AttachedWorld;

		Ref<Entity> m_ParentEntity;
		std::vector<Ref<Entity>> m_ChildEntities;

		std::vector<Ref<Component>> m_ComponentRegistry;

		NL_NODISCARD Ref<Entity> GetPointer() const;

		friend class World;
	};
}