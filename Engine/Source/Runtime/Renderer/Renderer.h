#pragma once

#include "Core/EngineAPI.h"
#include "Framebuffer.h"

namespace Nightly
{
	class Entity;
	class Shader;
	struct ShaderProgram;

	class NL_API Renderer
	{
	public:
		Renderer() = default;
		~Renderer();

		// Initializes the OpenGL library and necessary loads shaders.
		static void Initialize();
		static void Update();
		static void Cleanup();

		static std::shared_ptr<ShaderProgram> GetShaderProgram()
		{
			return m_ShaderProgram;
		}

	private:
		static void BeginFrame();
		static void EndFrame();
		static void ClearColor();

	private:
		static std::unique_ptr<Shader> m_VertexShader;
		static std::unique_ptr<Shader> m_FragmentShader;
		static std::shared_ptr<ShaderProgram> m_ShaderProgram;
		static std::unique_ptr<Entity> m_FallbackCamera;

		static Framebuffer m_Framebuffer;
	};
}
