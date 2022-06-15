#include "Framebuffer.h"

#include <glad/glad.h>

#include "Core/Core.h"

namespace Nightly
{
	void Framebuffer::Invalidate(const FramebufferProps& props)
	{
		if (props.IsHDR())
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, props.GetWidth(), props.GetHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, props.GetWidth(), props.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}

		// Renderbuffer
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, props.GetWidth(), props.GetHeight());
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Framebuffer);
	}

	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Framebuffer::Cleanup()
	{
		glDeleteFramebuffers(1, &m_Framebuffer);
		glDeleteBuffers(1, &m_RenderBuffer);
		glDeleteBuffers(1, &m_ColorBuffer);
	}

	void Framebuffer::Generate(const FramebufferProps& props)
	{
		glGenFramebuffers(1, &m_Framebuffer);

		// Color attachment texture
		glGenTextures(1, &m_ColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);

		if (props.IsHDR())
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, props.GetWidth(), props.GetHeight(), 0, GL_RGBA, GL_FLOAT, nullptr);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, props.GetWidth(), props.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Renderbuffer
		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, props.GetWidth(), props.GetHeight());

		Bind();
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		NL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to create framebuffer!", ENGINE);

		m_Props = props;
	}
}