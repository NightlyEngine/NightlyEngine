#include "CameraComponent.h"

namespace Nightly
{
	void CameraComponent::UpdateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
		}
		else
		{
			m_Projection = glm::ortho(m_Bounds.x, m_Bounds.y, m_Bounds.z, m_Bounds.w, m_Near, m_Far);
		}
	}
}