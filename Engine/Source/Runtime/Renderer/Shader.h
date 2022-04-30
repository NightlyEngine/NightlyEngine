#pragma once

#include "Core/EngineAPI.h"

namespace Nightly
{
	// An OpenGL shader object.
	class NL_API Shader
	{
	public:
		Shader(uint32_t type, std::string_view path)
				: m_Shader(), m_Type(type)
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
		void Compile(uint32_t type, std::string_view path);

		uint32_t m_Type;
		uint32_t m_Shader;
	};

	struct ShaderProgram
	{
		ShaderProgram();
		~ShaderProgram() = default;

		void Attach(const Shader* shader) const;
		void Link() const;
		void Use() const;
		void Delete() const;

	private:
		uint32_t m_Program;
	};
}
