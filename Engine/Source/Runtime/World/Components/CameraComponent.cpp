#include "CameraComponent.h"

#include "MeshComponent.h"

#include "Core/Window.h"
#include "Core/WindowManager.h"

#include "Renderer/Shader.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

#include "World/World.h"
#include "World/WorldManager.h"
#include "World/Entity.h"

namespace NL
{
	CameraComponent::CameraComponent(const CameraSpecification& specification)
			: Camera(specification)
	{
		// Create framebuffer
		int width = WindowManager::GetCurrentWindow()->GetWidth();
		int height = WindowManager::GetCurrentWindow()->GetHeight();

		FramebufferProps props(width, height, false);
		m_Framebuffer = Framebuffer(props);

		if (!m_ScreenTarget)
		{
			Framebuffer::Unbind();
		}
	}

	void CameraComponent::OnDraw(const ShaderProgram& shader)
	{
		RenderCommand::ClearColor(ClearColor());
		RenderCommand::Clear(OpenGLBufferBit::COLOR | OpenGLBufferBit::DEPTH);

		if (m_ScreenTarget)
		{
			m_Framebuffer.Bind();
		}

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

		if (m_ScreenTarget)
		{
			Framebuffer::Unbind();
			RenderCommand::ClearColor(ClearColor());
			RenderCommand::Clear(OpenGLBufferBit::COLOR | OpenGLBufferBit::DEPTH);
		}
	}
}