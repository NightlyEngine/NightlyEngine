#pragma once

#include "Component.h"

#include "Core/EngineAPI.h"
#include "Core/Math.h"
#include "Core/Core.h"

#include "Renderer/Camera.h"

namespace NL
{
	// The 'eye' of the player. Only one camera can draw to the screen at a time.
	class NL_API CameraComponent : public Camera, public Component
	{
	public:
		explicit CameraComponent(const CameraSpecification& specification);

		~CameraComponent() override = default;

		NL_DEFINE_COMPONENT(CameraComponent)

		void OnDraw(const ShaderProgram& shader) override;

		bool m_ScreenTarget = true;
	};
}
