#include "EditorCamera.h"

#include <glm/glm.hpp>

#include "Core/Event/InputSystem.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Shader.h"

#include "World/World.h"
#include "World/WorldManager.h"
#include "World/Entity.h"
#include "World/Components/MeshComponent.h"

#include "Core/Window.h"
#include "Core/WindowManager.h"

namespace NLE
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float near, float far)
			: Camera(CameraSpecification { fov, aspectRatio, near, far }), m_TargetPosition(m_Position)
	{
		// Create framebuffer
		int width = WindowManager::GetCurrentWindow()->GetWidth();
		int height = WindowManager::GetCurrentWindow()->GetHeight();

		FramebufferProps props(width, height, false);
		m_Framebuffer = Framebuffer(props);
	}

	void EditorCamera::OnDraw(const ShaderProgram& shader)
	{
		RenderCommand::ClearColor(ClearColor());
		RenderCommand::Clear(OpenGLBufferBit::COLOR | OpenGLBufferBit::DEPTH);

		m_Framebuffer.Bind();

		RenderCommand::EnableFeature(OpenGLFeature::DEPTH_TEST);
		RenderCommand::ClearColor(ClearColor());
		RenderCommand::Clear(OpenGLBufferBit::COLOR | OpenGLBufferBit::DEPTH);

		shader.Use();
		shader.SetUniformMat4("u_ProjectionView", GetProjectionViewMatrix());

		// TODO: Move mesh components into separate registry to increase performance
		for (const auto& entity : WorldManager::GetActiveWorld()->GetEntityRegistry())
		{
			auto mesh = entity->GetComponent<MeshComponent>();
			if (mesh)
			{
				mesh->Draw();
			}
		}

		RenderCommand::DisableFeature(OpenGLFeature::DEPTH_TEST);

		Framebuffer::Unbind();
		RenderCommand::ClearColor(ClearColor());
		RenderCommand::Clear(OpenGLBufferBit::COLOR | OpenGLBufferBit::DEPTH);
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		Vec3 direction(0.0f);

		if (InputSystem::IsKeyPressed(NL_KEY_W))
		{
			direction += m_Front;
		}

		if (InputSystem::IsKeyPressed(NL_KEY_S))
		{
			direction -= m_Front;
		}

		if (InputSystem::IsKeyPressed(NL_KEY_A))
		{
			direction -= glm::cross(m_Front, m_Up);
		}

		if (InputSystem::IsKeyPressed(NL_KEY_D))
		{
			direction += glm::cross(m_Front, m_Up);
		}

		if (InputSystem::IsKeyPressed(NL_KEY_Q))
		{
			direction -= m_Up;
		}

		if (InputSystem::IsKeyPressed(NL_KEY_E))
		{
			direction += m_Up;
		}

		// Have we moved?
		if (direction != Vec3(0.0f))
		{
			m_TargetPosition += glm::normalize(direction) * GetFlySpeed() * 0.01f;
		}
		
		m_Position = glm::mix(m_Position, m_TargetPosition, deltaTime * GetFlySpeed());
		m_View = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}
}