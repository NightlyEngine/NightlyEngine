#include "CameraComponent.h"

namespace Nightly
{
	void CameraComponent::Update()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			m_Projection = glm::perspective(glm::radians(m_FOV), 16.f / 9.f, m_Near, m_Far);
		}
		else
		{
			m_Projection = glm::ortho(m_Bounds.x, m_Bounds.y, m_Bounds.z, m_Bounds.w, m_Near, m_Far);
		}
	}
}