#pragma once

#include "Core/EngineAPI.h"
#include "Core/Math.h"

namespace NL
{
	// OpenGL capabilities.
	enum class OpenGLFeature
	{
		BLEND,
		DEPTH_TEST,
		STENCIL_TEST
	};

	enum OpenGLBufferBit : uint8_t
	{
		COLOR = NL_BIT(0),
		DEPTH = NL_BIT(1),
		STENCIL = NL_BIT(2)
	};

	class NL_API RenderCommand
	{
	public:
		RenderCommand() = default;
		~RenderCommand() = default;

		// Enables specified OpenGL capability.
		static void EnableFeature(const OpenGLFeature& feature);

		// Disables specified OpenGL capability.
		static void DisableFeature(const OpenGLFeature& feature);

		// Clears buffers to preset values.
		static void Clear(uint8_t bits);

		// Specifies the clear values for the color buffers.
		static void ClearColor(const Color& color);

	private:
		static int GetOpenGLCapability(const OpenGLFeature& feature);
		static int GetOpenGLBufferBit(uint8_t bufferBit);
	};
}
