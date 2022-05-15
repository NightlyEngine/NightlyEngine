#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Shader.h"

#include "World/Components/MeshComponent.h"
#include "World/Entity.h"
#include "World/World.h"
#include "World/WorldManager.h"

namespace Nightly
{
	std::unique_ptr<Shader> Renderer::m_VertexShader;
	std::unique_ptr<Shader> Renderer::m_FragmentShader;
	std::shared_ptr<ShaderProgram> Renderer::m_ShaderProgram;

	Renderer::~Renderer() = default;

	void Renderer::Initialize()
	{
		NL_CORE_INFO("Initializing Renderer...", ENGINE);
		NL_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize OpenGL library!", ENGINE);

		// FIXME: Shader file paths are invalid when engine gets shipped
		m_VertexShader = std::make_unique<Shader>(GL_VERTEX_SHADER, "../../Source/Shaders/UnlitVertexShader.glsl");
		m_FragmentShader = std::make_unique<Shader>(GL_FRAGMENT_SHADER, "../../Source/Shaders/UnlitFragmentShader.glsl");

		m_ShaderProgram = std::make_shared<ShaderProgram>();
		m_ShaderProgram->Attach(m_VertexShader.get());
		m_ShaderProgram->Attach(m_FragmentShader.get());
		m_ShaderProgram->Link();
		m_ShaderProgram->Use();
	}

	void Renderer::Update()
	{
		m_ShaderProgram->Use();

		// TODO: Move mesh components into separate registry to increase performance
		for (const auto& entity : WorldManager::GetActiveWorld()->m_EntityRegistry)
		{
			auto mesh = entity->GetComponent<MeshComponent>();
			if (mesh)
			{
				mesh->Draw();
			}
		}
	}
}