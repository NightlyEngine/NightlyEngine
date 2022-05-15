#include "Entity.h"

#include "World.h"
#include "Components/TransformComponent.h"

namespace Nightly
{
	Entity::~Entity() = default;

	void Entity::Initialize(const std::shared_ptr<World>& attachedWorld)
	{
		if (Transform()) return;

		m_AttachedWorld = attachedWorld;

		auto component = std::make_shared<TransformComponent>();
		component->m_AttachedEntity = GetPointer();
		m_ComponentRegistry.push_back(component);
	}

	std::shared_ptr<Entity> Entity::GetPointer() const
	{
		if (!m_AttachedWorld) return nullptr;
		
		return m_AttachedWorld->FindEntityById(GetUUID());
	}
}