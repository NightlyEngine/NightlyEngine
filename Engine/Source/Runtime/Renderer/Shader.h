#pragma once

#include "Core/EngineAPI.h"
#include "Core/Math.h"

namespace NL
{
	// An OpenGL shader object.
	class NL_API Shader
	{
	public:
		Shader() = default;

		Shader(uint32_t type, const std::string& path)
				: m_Type(type), m_Shader()
		{
			Compile(type, path);
		}

		~Shader() = default;

		// Calls glDeleteShader on this object.
		void Delete() const;

		explicit operator uint32_t() const
		{
			return m_Shader;
		}

	private:
		void Compile(uint32_t type, const std::string& path);

		uint32_t m_Type;
		uint32_t m_Shader;
	};

	struct ShaderProgram
	{
		ShaderProgram()
				: m_Program(), m_IsInitialized(false)
		{
		}

		~ShaderProgram() = default;

		void Initialize();
		void Attach(const Shader& shader) const;
		void Link() const;
		void Use() const;
		void Delete() const;

		void SetUniform3fv(const char* name, const Vec3& value) const;
		void SetUniform4fv(const char* name, const Vec3& value) const;
		void SetUniformMatrix4fv(const char* name, const Mat4& value) const;

	private:
		uint32_t m_Program;
		bool m_IsInitialized;
	};
}
