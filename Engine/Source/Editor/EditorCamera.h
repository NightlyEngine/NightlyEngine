#pragma once

#include "Core/Core.h"
#include "Core/Math.h"
#include "Renderer/Camera.h"

using namespace NL;

namespace NLE
{
	class EditorCamera : public Camera
	{
	public:
		explicit EditorCamera(float fov = 70.0f, float aspectRatio = 16.0f / 9.0f, float near = 0.1f, float far = 500.0f);

		~EditorCamera() override = default;

		void OnDraw(const ShaderProgram& shader) override;
		void OnUpdate(float deltaTime);

		// Returns the calculated movement speed of the camera.
		NL_NODISCARD inline float GetFlySpeed() const
		{
			return m_MovementSpeed * m_SpeedMultiplier;
		}

	private:
		Vec3 m_Position = Vec3(0.0f);
		Vec3 m_TargetPosition;

		float m_MovementSpeed = 3.0f;
		float m_SpeedMultiplier = 0.5f;

		Vec3 m_Up = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 m_Front = Vec3(0.0f, 0.0f, -1.0f);

		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
	};
}
