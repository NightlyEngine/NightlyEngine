#pragma once

#include "Core/EngineAPI.h"
#include "Shader.h"

namespace Nightly
{
	class NL_API Renderer
	{
	public:
		// Initializes the OpenGL library and necessary loads shaders.
		static void Initialize();

	private:
		static inline std::unique_ptr<Shader> m_VertexShader, m_FragmentShader;
		static inline std::unique_ptr<ShaderProgram> m_ShaderProgram;
	};
}
