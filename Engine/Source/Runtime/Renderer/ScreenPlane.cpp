#include "ScreenPlane.h"

#include <glad/glad.h>

#include "Shader.h"

namespace NL
{
	void ScreenPlane::Initialize()
	{
		// FIXME: Shader file paths are invalid when engine gets shipped, see Renderer.cpp
		std::string vertexPath = "../../Source/Shaders/ScreenVertexShader.glsl";
		std::string fragmentPath = "../../Source/Shaders/ScreenFragmentShader.glsl";
		
		m_ScreenShader = ShaderProgram(vertexPath, fragmentPath);
		m_ScreenShader.Use();

		// Plane vertices
		std::array<float, 24> vertices = {
				-1.0f, 1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				1.0f, -1.0f, 1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, -1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f
		};

		// Generate buffers
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		// Bind buffers
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		// Set vertex positions
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) nullptr);
		glEnableVertexAttribArray(0);

		// Set uv coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	void ScreenPlane::Update(uint32_t renderTexture)
	{
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		m_ScreenShader.Use();
		glBindVertexArray(m_VAO);
		glBindTexture(GL_TEXTURE_2D, renderTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void ScreenPlane::Cleanup()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}
}