#pragma once

#include "Core/EngineAPI.h"

namespace Nightly
{
	struct ShaderProgram;

	// Draws the framebuffer on the entire screen so that
	// post-processing effects can be applied.
	// Used for standalone applications, since there is
	// no way of running the framebuffer
	// through a shader and displaying it by default.
	class NL_API ScreenPlane
	{
	public:
		ScreenPlane() = default;
		~ScreenPlane() = default;

		static void Initialize();
		static void Update(uint32_t renderTexture);
		static void Cleanup();

	private:
		static inline uint32_t m_VAO = 0;
		static inline uint32_t m_VBO = 0;

		static std::unique_ptr<ShaderProgram> m_ScreenShader;
	};
}
