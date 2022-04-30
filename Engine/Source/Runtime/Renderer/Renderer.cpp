#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Shader.h"

namespace Nightly
{
	void Renderer::Initialize()
	{
		NL_CORE_INFO("Initializing Renderer...", ENGINE);
		NL_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize OpenGL library!", ENGINE);

		// FIXME: Shader file paths are invalid when engine gets shipped
		m_VertexShader = std::make_unique<Shader>(GL_VERTEX_SHADER, "../../Source/Shaders/UnlitVertexShader.glsl");
		m_FragmentShader = std::make_unique<Shader>(GL_FRAGMENT_SHADER, "../../Source/Shaders/UnlitFragmentShader.glsl");

		m_ShaderProgram = std::make_unique<ShaderProgram>();
		m_ShaderProgram->Attach(m_VertexShader.get());
		m_ShaderProgram->Attach(m_FragmentShader.get());
		m_ShaderProgram->Link();
		m_ShaderProgram->Use();
	}
}