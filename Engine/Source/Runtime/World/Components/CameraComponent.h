#pragma once

#include "Component.h"
#include "Core/EngineAPI.h"
#include "Core/Math.h"
#include "Core/Core.h"

namespace NL
{
	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	// The 'eye' of the player. Only one camera can draw to the screen at a time.
	class NL_API CameraComponent : public Component
	{
	public:
		CameraComponent(float fov, float aspectRatio, float near, float far)
				: m_ProjectionType(ProjectionType::Perspective),
				  m_FOV(fov > 0.0f ? fov : 60.0f), m_Near(near), m_Far(far),
				  m_AspectRatio(aspectRatio), m_Bounds(0.0f),
				  m_Projection(glm::perspective(glm::radians(fov), 1.6f, near, far))
		{
		}

		CameraComponent(float left, float right, float bottom, float top, float near, float far)
				: m_ProjectionType(ProjectionType::Orthographic),
				  m_FOV(), m_Near(near),
				  m_Far(far), m_AspectRatio(), m_Bounds(left, right, bottom, top),
				  m_Projection(glm::ortho(left, right, bottom, top, near, far))
		{
		}

		~CameraComponent() override = default;

		NL_DEFINE_COMPONENT(CameraComponent)

		// Refreshes the projection matrix of the camera.
		// Don't call this every frame since it gets called
		// automatically once any setting changes.
		virtual void UpdateProjection();

		// Returns the field of view of the camera.
		NL_NODISCARD float GetFOV() const
		{
			return m_FOV;
		}

		// Sets the field of view to a specified value.
		// FOV must be greater than zero.
		void SetFOV(float fov)
		{
			if (fov > 0) m_FOV = fov;
			UpdateProjection();
		}

		NL_NODISCARD ProjectionType GetProjectionType() const
		{
			return m_ProjectionType;
		}

		void SetProjectionType(const ProjectionType& type)
		{
			m_ProjectionType = type;
			UpdateProjection();
		}

		NL_NODISCARD float GetNearClippingPlanes() const
		{
			return m_Near;
		}

		void SetNearClippingPlanes(float near)
		{
			if (near >= 0.0f && near < m_Far) m_Near = near;
			UpdateProjection();
		}

		NL_NODISCARD float GetFarClippingPlanes() const
		{
			return m_Far;
		}

		void SetFarClippingPlanes(float far)
		{
			if (far > m_Near) m_Far = far;
			UpdateProjection();
		}

		NL_NODISCARD float GetAspectRatio() const
		{
			return m_AspectRatio;
		}

		void SetAspectRatio(float ratio)
		{
			if (ratio > 0) m_AspectRatio = ratio;
			UpdateProjection();
		}

		// Returns the viewport bounds if the projection type is set to orthographic.
		NL_NODISCARD Vec4 GetBounds() const
		{
			return m_Bounds;
		}

		// Sets the viewport bounds if the projection type is set to orthographic.
		void SetBounds(const Vec4& bounds)
		{
			m_Bounds = bounds;
			UpdateProjection();
		}

		// Returns the view matrix of the camera.
		NL_NODISCARD Mat4 GetView() const
		{
			return m_View;
		}

		// Returns the projection matrix of the camera.
		NL_NODISCARD Mat4 GetProjection() const
		{
			return m_Projection;
		}

	private:
		ProjectionType m_ProjectionType;

		float m_FOV;
		float m_Near;
		float m_Far;
		float m_AspectRatio;

		Vec4 m_Bounds;

		Mat4 m_View = Mat4(1.0f);
		Mat4 m_Projection;
	};
}
