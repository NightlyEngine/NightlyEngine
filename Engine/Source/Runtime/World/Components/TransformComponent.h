#pragma once

#include "Component.h"
#include "Core/EngineAPI.h"
#include "Core/Math.h"

namespace NL
{
	// Contains information like position, rotation and scale.
	class NL_API TransformComponent : public Component
	{
	public:
		TransformComponent() = default;
		~TransformComponent() override = default;

		NL_DEFINE_COMPONENT(TransformComponent)

		// Resets the position, rotation and scale to their default values.
		void Reset()
		{
			m_Position = Vec3(0.0f);
			m_Rotation = Vec3(0.0f);
			m_Scale = Vec3(1.0f);
		}

		void Translate(const Vec3& position);
		void Rotate(const Vec3& rotation);
		void Scale(const Vec3& scale);

		// Returns the transformation matrix.
		NL_NODISCARD Mat4

		GetTransform() const
		{
			Vec3 rotation = glm::radians(m_Rotation);
			Mat4 transform = Mat4(1.0f);

			transform = glm::toMat4(glm::quat(rotation));
			transform = glm::translate(transform, m_Position);
			transform = glm::scale(transform, m_Scale);

			return transform;
		}

		void SetPosition(const Vec3& position)
		{
			m_Position = position;
		}

		NL_NODISCARD Vec3

		GetPosition() const
		{
			return m_Position;
		}

		void SetRotation(const Vec3& rotation)
		{
			m_Rotation = rotation;
		}

		NL_NODISCARD Vec3

		GetRotation() const
		{
			return m_Rotation;
		}

		void SetScale(const Vec3 scale)
		{
			m_Scale = scale;
		}

		NL_NODISCARD Vec3

		GetScale() const
		{
			return m_Scale;
		}

	private:
		Vec3 m_Position = Vec3(0.0f);
		Vec3 m_Rotation = Vec3(0.0f);
		Vec3 m_Scale = Vec3(1.0f);
	};
}
