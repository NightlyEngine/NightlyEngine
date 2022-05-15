#pragma once

#include "Component.h"
#include "Core/EngineAPI.h"
#include "Core/Math.h"

namespace Nightly
{
	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	class NL_API CameraComponent : public Component
	{
	public:
		CameraComponent(float fov, float near, float far)
				: m_Projection(glm::perspective(glm::radians(fov), 1.6f, near, far)),
				  m_ProjectionType(ProjectionType::Perspective), m_Bounds(0.0f), m_FOV(fov > 0 ? fov : 60), m_Near(near), m_Far(far)
		{
		}

		CameraComponent(float left, float right, float bottom, float top, float near, float far)
				: m_Projection(glm::ortho(left, right, bottom, top, near, far)),
				  m_ProjectionType(ProjectionType::Orthographic), m_Bounds(left, right, bottom, top),
				  m_FOV(), m_Near(near), m_Far(far)
		{
		}

		~CameraComponent() override = default;

		NL_DEFINE_COMPONENT(CameraComponent)

		virtual void Update();

		// Returns the field of view of the camera.
		[[nodiscard]] float GetFOV() const
		{
			return m_FOV;
		}

		// Sets the field of view to a specified value.
		// FOV must be greater than zero.
		void SetFOV(float fov)
		{
			if (fov > 0) m_FOV = fov;
		}

		[[nodiscard]] ProjectionType GetProjectionType() const
		{
			return m_ProjectionType;
		}

		[[nodiscard]] float GetNearClippingPlanes() const
		{
			return m_Near;
		}

		void SetNearClippingPlanes(float near)
		{
			if (near >= 0.0f && near < m_Far) m_Near = near;
		}

		[[nodiscard]] float GetFarClippingPlanes() const
		{
			return m_Far;
		}

		void SetFarClippingPlanes(float far)
		{
			if (far > m_Near) m_Far = far;
		}

		// Returns the view matrix of the camera.
		[[nodiscard]] Mat4 GetView() const
		{
			return m_View;
		}

		// Returns the projection matrix of the camera.
		[[nodiscard]] Mat4 GetProjection() const
		{
			return m_Projection;
		}

	private:
		ProjectionType m_ProjectionType;

		float m_FOV;
		float m_Near;
		float m_Far;

		Vec4 m_Bounds;

		Mat4 m_View = Mat4(1.0f);
		Mat4 m_Projection;
	};
}
