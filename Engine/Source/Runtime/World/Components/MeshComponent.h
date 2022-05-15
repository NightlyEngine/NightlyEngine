#pragma once

#include "Component.h"

namespace Nightly
{
	class MeshComponent : public Component
	{
	public:
		MeshComponent()
		{
			Setup();
		}

		~MeshComponent() override = default;

		NL_DEFINE_COMPONENT(MeshComponent)

		void Draw() const;
		void OnDestroy() override;

	private:
		void Setup();

		uint32_t m_VAO = 0;
		uint32_t m_VBO = 0;
		uint32_t m_EBO = 0;
	};
}
