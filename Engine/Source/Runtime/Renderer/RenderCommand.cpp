#include "RenderCommand.h"

#include <glad/glad.h>

namespace NL
{
	void RenderCommand::EnableFeature(const OpenGLFeature& feature)
	{
		glEnable(GetOpenGLCapability(feature));
	}

	void RenderCommand::DisableFeature(const OpenGLFeature& feature)
	{
		glDisable(GetOpenGLCapability(feature));
	}

	void RenderCommand::Clear(uint8_t bits)
	{
		glClear(GetOpenGLBufferBit(bits));
	}

	void RenderCommand::ClearColor(const Color& color)
	{
		glClearColor(NL_UNWRAP_XYZW(Vec4(color)));
	}

	int RenderCommand::GetOpenGLCapability(const OpenGLFeature& feature)
	{
		switch (feature)
		{
			case OpenGLFeature::BLEND: return GL_BLEND;
			case OpenGLFeature::DEPTH_TEST: return GL_DEPTH_TEST;
			case OpenGLFeature::STENCIL_TEST: return GL_STENCIL_TEST;
		}
		
		NL_ASSERT(false, "Unknown OpenGL feature!", ENGINE);
		return -1;
	}

	int RenderCommand::GetOpenGLBufferBit(uint8_t bufferBit)
	{
		int result = 0;

		if (bufferBit & COLOR) result |= GL_COLOR_BUFFER_BIT;
		if (bufferBit & DEPTH) result |= GL_DEPTH_BUFFER_BIT;
		if (bufferBit & STENCIL) result |= GL_STENCIL_BUFFER_BIT;

		return result;
	}
}
