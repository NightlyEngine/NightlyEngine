#include "Framebuffer.h"

#include <glad/glad.h>

#include "Core/Core.h"

namespace Nightly
{
	void Framebuffer::Setup()
	{
		glGenFramebuffers(1, &m_Framebuffer);

		// TODO: Update resolution to match window size
		int width = 1280;
		int height = 720;

		// Color attachment texture
		glGenTextures(1, &m_ColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Renderbuffer
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

		Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		NL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to create framebuffer!", ENGINE);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
}