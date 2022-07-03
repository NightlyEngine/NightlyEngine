#include "Entity.h"

#include "World.h"
#include "Components/TransformComponent.h"

namespace NL
{
	Entity::~Entity() = default;

	void Entity::Initialize(const Ref<World>& attachedWorld)
	{
		if (Transform()) return;

		m_AttachedWorld = attachedWorld;

		auto component = Component::Create<TransformComponent>();
		component->m_AttachedEntity = GetPointer();
		m_ComponentRegistry.push_back(component);
	}

	Ref<Entity> Entity::GetPointer() const
	{
		if (!m_AttachedWorld) return nullptr;

		return m_AttachedWorld->FindEntityById(GetUUID());
	}
}