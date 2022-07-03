#pragma once

#include "Framebuffer.h"

#include "Core/EngineAPI.h"
#include "Core/Math.h"
#include "Core/Core.h"

namespace NL
{
	// Camera projection types.
	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	struct CameraSpecification
	{
		CameraSpecification() = default;

		CameraSpecification(float fov, float aspectRatio, float near, float far)
				: m_FOV(fov), m_AspectRatio(aspectRatio), m_Bounds(),
				  m_Near(near), m_Far(far), m_ProjectionType(ProjectionType::Perspective)
		{
		}

		CameraSpecification(float left, float right, float bottom, float top, float near, float far)
				: m_FOV(70.0f), m_AspectRatio(), m_Bounds(left, right, bottom, top), m_Near(near), m_Far(far),
				  m_ProjectionType(ProjectionType::Orthographic)
		{
		}

		float m_FOV;
		float m_AspectRatio;

		Bounds2D m_Bounds;

		float m_Near;
		float m_Far;

		ProjectionType m_ProjectionType;
		Color m_ClearColor;
	};

	// Base camera class.
	class NL_API Camera
	{
	public:
		explicit Camera(CameraSpecification specification);

		virtual ~Camera() = default;

		virtual void OnDraw(const struct ShaderProgram& shader) = 0;

		// Gets called automatically once any setting changes
		// to update the projection matrix of the camera.
		virtual void OnProjectionChange();

		// Returns the field of view of the camera.
		inline float GetFOV()
		{
			if (ProjectionType() == ProjectionType::Perspective)
			{
				return m_Specification.m_FOV;
			}

			return 0;
		}

		// Sets the field of view to a specified value.
		// FOV must be greater than zero.
		inline void SetFOV(float fov)
		{
			if (ProjectionType() == ProjectionType::Perspective && fov > 0 && fov <= MAX_FOV)
			{
				m_Specification.m_FOV = fov;
				OnProjectionChange();
			}
		}

		inline float GetAspectRatio()
		{
			if (ProjectionType() == ProjectionType::Perspective)
			{
				return m_Specification.m_AspectRatio;
			}

			return 0;
		}

		inline void SetAspectRatio(float aspect)
		{
			if (ProjectionType() == ProjectionType::Perspective && aspect != 0)
			{
				m_Specification.m_AspectRatio = aspect;
				OnProjectionChange();
			}
		}

		NL_NODISCARD inline float GetNearClipPlanes() const
		{
			return m_Specification.m_Near;
		}

		inline void SetNearClipPlanes(float near)
		{
			if (near > 0 && near < MAX_FAR_CLIP_PLANES)
			{
				m_Specification.m_Near = near;
				OnProjectionChange();
			}
		}

		NL_NODISCARD inline float GetFarClipPlanes() const
		{
			return m_Specification.m_Far;
		}

		inline void SetFarClipPlanes(float far)
		{
			if (far > GetNearClipPlanes() && far <= MAX_FAR_CLIP_PLANES)
			{
				m_Specification.m_Far = far;
				OnProjectionChange();
			}
		}

		NL_NODISCARD inline Bounds2D GetBounds() const
		{
			return m_Specification.m_Bounds;
		}

		inline ProjectionType& ProjectionType()
		{
			return m_Specification.m_ProjectionType;
		}

		inline Color& ClearColor()
		{
			return m_Specification.m_ClearColor;
		}

		NL_NODISCARD const Framebuffer& GetFramebuffer() const
		{
			return m_Framebuffer;
		}

		// Returns the view matrix of the camera.
		NL_NODISCARD const Mat4& GetView() const
		{
			return m_View;
		}

		// Returns the projection matrix of the camera.
		NL_NODISCARD const Mat4& GetProjection() const
		{
			return m_Projection;
		}

		NL_NODISCARD Mat4 GetProjectionViewMatrix() const
		{
			return m_Projection * m_View;
		}

	protected:
		static Mat4 CreateProjection(const CameraSpecification& specification);

		CameraSpecification m_Specification;
		Framebuffer m_Framebuffer;

		Mat4 m_View;
		Mat4 m_Projection;

		static constexpr float MAX_FAR_CLIP_PLANES = 10000000.0f;            // Maximum far clip planes: 10M
		static constexpr float MAX_FOV = 180.0f;
	};
}
