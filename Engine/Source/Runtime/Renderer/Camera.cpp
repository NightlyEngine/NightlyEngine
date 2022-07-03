#include "Camera.h"

namespace NL
{
	Camera::Camera(CameraSpecification specification)
			: m_Specification(specification), m_View(1.0f), m_Projection()
	{
		m_Projection = CreateProjection(specification);
	}

	void Camera::OnProjectionChange()
	{
		m_Projection = CreateProjection(m_Specification);
	}

	Mat4 Camera::CreateProjection(const CameraSpecification& specification)
	{
		if (specification.m_ProjectionType == ProjectionType::Perspective)
		{
			return glm::perspective(glm::radians(specification.m_FOV), specification.m_AspectRatio,
			                        specification.m_Near, specification.m_Far);
		}
		else
		{
			return glm::ortho(NL_UNWRAP_XYZW(specification.m_Bounds.GetBounds()));
		}
	}
}