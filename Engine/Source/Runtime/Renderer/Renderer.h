#pragma once

#include "Core/EngineAPI.h"
#include "Core/Event/Event.h"
#include "Framebuffer.h"
#include "Shader.h"

namespace Nightly
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

	private:
		static void BeginFrame();
		static void EndFrame();
		static void ClearColor();
		static void InvalidateFramebuffer(EventFun fun);

	private:
		static inline Shader m_VertexShader;
		static inline Shader m_FragmentShader;
		static inline ShaderProgram m_ShaderProgram;
		static std::unique_ptr<Entity> m_FallbackCamera;

		static Framebuffer m_Framebuffer;
	};
}
