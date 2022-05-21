#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/WindowManager.h"
#include "Core/Window.h"
#include "Shader.h"
#include "ScreenPlane.h"

#include "World/Components/MeshComponent.h"
#include "World/Entity.h"
#include "World/World.h"
#include "World/WorldManager.h"
#include "World/Components/CameraComponent.h"

#include "Core/Event/EventSystem.h"
#include "Core/Event/WindowEvents.h"

namespace Nightly
{
	std::unique_ptr<Shader> Renderer::m_VertexShader;
	std::unique_ptr<Shader> Renderer::m_FragmentShader;
	std::shared_ptr<ShaderProgram> Renderer::m_ShaderProgram;
	std::unique_ptr<Entity> Renderer::m_FallbackCamera;

	Framebuffer Renderer::m_Framebuffer;

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

		m_FallbackCamera = std::make_unique<Entity>("Fallback Camera");
		m_FallbackCamera->Initialize(nullptr);

		float aspect = WindowManager::GetCurrentWindow()->GetAspectRatio();
		m_FallbackCamera->AddComponent<CameraComponent>(std::make_shared<CameraComponent>(60, aspect, 0.1f, 1000.0f));

		m_Framebuffer.Setup();

		ScreenPlane::Initialize();

		EventSystem::Subscribe(Events::OnFramebufferResize::EventType, InvalidateFramebuffer);
	}

	void Renderer::Update()
	{
		BeginFrame();

		m_ShaderProgram->Use();
		m_ShaderProgram->SetUniformMatrix4fv("uView", m_FallbackCamera->GetComponent<CameraComponent>()->GetView());
		m_ShaderProgram->SetUniformMatrix4fv("uProjection", m_FallbackCamera->GetComponent<CameraComponent>()->GetProjection());

		// TODO: Move mesh components into separate registry to increase performance
		for (const auto& entity : WorldManager::GetActiveWorld()->m_EntityRegistry)
		{
			auto mesh = entity->GetComponent<MeshComponent>();
			if (mesh)
			{
				mesh->Draw();
			}
		}

		EndFrame();

		ScreenPlane::Update(m_Framebuffer.GetColorBuffer());
	}

	void Renderer::Cleanup()
	{
		ScreenPlane::Cleanup();
		m_Framebuffer.Cleanup();
	}

	void Renderer::BeginFrame()
	{
		m_Framebuffer.Bind();
		glEnable(GL_DEPTH_TEST);
		ClearColor();
	}

	void Renderer::EndFrame()
	{
		Framebuffer::Unbind();
		glDisable(GL_DEPTH_TEST);
	}

	void Renderer::ClearColor()
	{
		glClearColor(27 / 255.f, 25 / 255.f, 50 / 255.f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::InvalidateFramebuffer(EventFun fun)
	{
		auto event = NL_CAST_EVENT(OnFramebufferResize, fun);

		if (event)
		{
			glViewport(0, 0, event->GetWidth(), event->GetHeight());
			m_Framebuffer.Invalidate(event->GetWidth(), event->GetHeight());

			float ratio = (float) event->GetWidth() / (float) event->GetHeight();
			m_FallbackCamera->GetComponent<CameraComponent>()->SetAspectRatio(ratio);
		}
	}
}