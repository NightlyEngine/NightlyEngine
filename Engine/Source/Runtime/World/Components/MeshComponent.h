#pragma once

#include "Component.h"
#include "Core/EngineAPI.h"

namespace NL
{
	struct ShaderProgram;

	class NL_API MeshComponent : public Component
	{
	public:
		MeshComponent()
		{
			Setup();
		}

		~MeshComponent() override = default;

		NL_DEFINE_COMPONENT(MeshComponent)

		void Draw();
		void OnDestroy() override;

	private:
		void Setup();

		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;
	};
}
