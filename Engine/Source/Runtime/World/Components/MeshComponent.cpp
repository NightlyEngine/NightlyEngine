#include "MeshComponent.h"

#include <glad/glad.h>
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "World/Entity.h"

namespace Nightly
{
	void MeshComponent::Setup()
	{
		std::array<float, 12> vertices = {
				0.5f, 0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				-0.5f, -0.5f, 0.0f,
				-0.5f, 0.5f, 0.0f
		};

		std::array<uint32_t, 6> indices = {
				0, 1, 3,
				1, 2, 3
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void MeshComponent::Draw()
	{
		Renderer::GetShaderProgram()->SetUniformMatrix4fv("uModel", GetEntity()->Transform()->GetTransform());

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}

	void MeshComponent::OnDestroy()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}
}
