#include "TransformComponent.h"

#include "World/Entity.h"

namespace Nightly
{
	void TransformComponent::Translate(const Vec3& position)
	{
		m_Position += position;
	}

	void TransformComponent::Rotate(const Vec3& rotation)
	{
		m_Rotation += rotation;
	}

	void TransformComponent::Scale(const Vec3& scale)
	{
		m_Scale += scale;
	}
}