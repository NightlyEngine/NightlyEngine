#pragma once

#include "Core/EngineAPI.h"

namespace Nightly
{
	class Shader;
	struct ShaderProgram;

	class NL_API Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		// Initializes the OpenGL library and necessary loads shaders.
		static void Initialize();

		static void Update();

	private:
		static std::unique_ptr<Shader> m_VertexShader;
		static std::unique_ptr<Shader> m_FragmentShader;
		static std::unique_ptr<ShaderProgram> m_ShaderProgram;
	};
}
