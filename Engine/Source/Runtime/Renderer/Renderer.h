#pragma once

#include "Core/EngineAPI.h"
#include "Core/Event/Event.h"
#include "Framebuffer.h"
#include "Shader.h"

namespace NL
{
	class Entity;

	class NL_API Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		// Initializes the OpenGL library and necessary loads shaders.
		static void Initialize();

		// Renders the current world.
		static void Update();
		static void Cleanup();

		static ShaderProgram GetShaderProgram()
		{
			return m_ShaderProgram;
		}

		static Framebuffer GetActiveFramebuffer();

		static const Vec2& GetViewportSize()
		{
			return m_ViewportSize;
		}

		static inline void SetViewportSize(const Vec2& size)
		{
			if (size.x > 0 && size.y > 0)
			{
				m_ViewportSize = size;
				InvalidateViewport();
			}
		}

		static inline bool m_DrawFramebufferToScreen = false;

	private:
		static void InvalidateViewport();
		static void InvalidateFramebuffer(EventFun fun);

	private:
		static inline ShaderProgram m_ShaderProgram;
		static Scope<Entity> m_FallbackCamera;

		static inline Vec2 m_ViewportSize;
	};
}
