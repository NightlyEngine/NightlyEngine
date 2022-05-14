#pragma once

#include "Component.h"
#include "Core/Math.h"

namespace Nightly
{
	// Contains information like position, rotation and scale.
	class TransformComponent : public Component
	{
	public:
		TransformComponent() = default;
		~TransformComponent() override = default;

		NL_DEFINE_COMPONENT(TransformComponent)

		// Returns the transformation matrix.
		[[nodiscard]] Mat4 GetTransform() const
		{
			Vec3 rotation = glm::radians(m_Rotation);
			Mat4 transform = Mat4(1.0f);

			transform = glm::toMat4(glm::quat(rotation));
			transform = glm::translate(transform, m_Position);
			transform = glm::scale(transform, m_Scale);

			return transform;
		}

		Vec3 m_Position = Vec3(0.0f);
		Vec3 m_Rotation = Vec3(0.0f);
		Vec3 m_Scale = Vec3(1.0f);
	};
}
