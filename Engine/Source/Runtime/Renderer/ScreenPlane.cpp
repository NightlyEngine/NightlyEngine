#include "ScreenPlane.h"

#include <glad/glad.h>

#include "Shader.h"

namespace Nightly
{
	void ScreenPlane::Initialize()
	{
		// FIXME: Shader file paths are invalid when engine gets shipped, see Renderer.cpp
		auto vertexShader = Shader(GL_VERTEX_SHADER, "../../Source/Shaders/ScreenVertexShader.glsl");
		auto fragmentShader = Shader(GL_FRAGMENT_SHADER, "../../Source/Shaders/ScreenFragmentShader.glsl");

		m_ScreenShader.Initialize();
		m_ScreenShader.Attach(vertexShader);
		m_ScreenShader.Attach(fragmentShader);
		m_ScreenShader.Link();
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
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) nullptr);

		// Set uv coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
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