#include "Renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Core.h"
#include "Core/Log.h"
#include "Core/WindowManager.h"
#include "Core/Window.h"
#include "Core/Event/EventSystem.h"
#include "Core/Event/WindowEvents.h"

#include "Shader.h"
#include "ScreenPlane.h"

#include "World/Entity.h"
#include "World/World.h"
#include "World/Components/CameraComponent.h"

namespace NL
{
	Scope<Entity> Renderer::m_FallbackCamera;

	Renderer::~Renderer() = default;

	void Renderer::Initialize()
	{
		NL_CORE_INFO("Initializing Renderer...", ENGINE);
		NL_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "Failed to initialize OpenGL library!", ENGINE);

		// Setup shaders
		{
			// FIXME: Shader file paths are invalid when engine gets shipped
			std::string vertexPath = "../../Source/Shaders/UnlitVertexShader.glsl";
			std::string fragmentPath = "../../Source/Shaders/UnlitFragmentShader.glsl";
			
			m_ShaderProgram = ShaderProgram(vertexPath, fragmentPath);
			m_ShaderProgram.Use();
		}

		// Create viewport camera
		{
			m_FallbackCamera = MakeScope<Entity>("Fallback Camera");
			m_FallbackCamera->Initialize(nullptr);

			float aspect = WindowManager::GetCurrentWindow()->GetAspectRatio();
			CameraSpecification specification(60.0f, aspect, 0.1f, 1000.0f);

			auto camera = Component::Create<CameraComponent>(specification);
			m_FallbackCamera->AddComponent<CameraComponent>(camera);
		}

		ScreenPlane::Initialize();

		EventSystem::Subscribe(Events::OnFramebufferResize::EventType, InvalidateFramebuffer);
	}

	void Renderer::Update()
	{
		m_FallbackCamera->GetComponent<CameraComponent>()->OnDraw(m_ShaderProgram);

		// Draw to screen if enabled
		if (m_FallbackCamera->GetComponent<CameraComponent>()->m_ScreenTarget)
		{
			ScreenPlane::Update(GetActiveFramebuffer().GetColorBuffer());
		}
	}

	void Renderer::Cleanup()
	{
		ScreenPlane::Cleanup();

		// TODO: Clean up ALL framebuffers, not just the active one
		GetActiveFramebuffer().Cleanup();
	}

	Framebuffer Renderer::GetActiveFramebuffer()
	{
		return m_FallbackCamera->GetComponent<CameraComponent>()->GetFramebuffer();
	}

	void Renderer::InvalidateViewport()
	{
		// Update viewport camera's aspect ratio
		float ratio = m_ViewportSize.x / m_ViewportSize.y;
		m_FallbackCamera->GetComponent<CameraComponent>()->SetAspectRatio(ratio);
	}

	void Renderer::InvalidateFramebuffer(EventFun fun)
	{
		if (!m_DrawFramebufferToScreen) return;

		auto event = NL_CAST_EVENT(OnFramebufferResize, fun);
		if (event)
		{
			// Resize OpenGL viewport
			glViewport(0, 0, event->GetWidth(), event->GetHeight());

			// Update viewport camera's aspect ratio
			float ratio = (float) event->GetWidth() / (float) event->GetHeight();
			m_FallbackCamera->GetComponent<CameraComponent>()->SetAspectRatio(ratio);

			// Recreate framebuffer
			FramebufferProps props(event->GetWidth(), event->GetHeight(), false);
			GetActiveFramebuffer().Invalidate(props);
		}
	}
}